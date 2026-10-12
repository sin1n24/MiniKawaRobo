# -*- coding: utf-8 -*-
"""各ページの OGP 画像 (1200x630) を assets/ogp/ に生成する。

使い方 (リポジトリ直下で):  python _tools/make_ogp.py
ページを増やしたら PAGES に1行足して実行し、そのページの front matter に
  image: /assets/ogp/<slug>.jpg
を書く。_tools/ は先頭が _ なので Jekyll の公開対象にならない。
"""
import os
from PIL import Image, ImageDraw, ImageFont

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "assets", "ogp")
W, H = 1200, 630
LEFT = 520                      # 左の文字パネルの幅
YELLOW = (255, 200, 40)
ORANGE = (240, 120, 20)
INK = (40, 28, 16)
FONT_DIR = r"C:\Windows\Fonts"
FONT_BOLD = ["NotoSansJP-Bold.ttf", "meiryob.ttc", "YuGothB.ttc"]

# slug: (見出し, 小見出し, 右側の画像 [複数なら2列のコラージュ], 合わせ方)
#   cover = 枠いっぱいに切り抜き / contain = 全体を収める
PAGES = {
    "default":  ("ミニかわロボ", "手のひらサイズの\n格闘ロボット競技", ["assets/Sticker.png"], "cover"),
    "about":    ("ミニかわロボ\nとは", "概要と基本ルール", ["assets/MiniKawaRobo.png"], "cover"),
    "howto":    ("つくりかた", "電池・制御・操縦・\nサーボ・フレーム", ["assets/howto.png"], "contain"),
    "rule":     ("競技ルール", "サイズ・重量・\n勝敗の定義", ["assets/rule.png"], "contain"),
    "archive":  ("ミニ大会", "各地で開催している\n大会の記録と予定", ["assets/hero.jpg"], "cover"),
    "workshop": ("ワーク\nショップ", "組立体験と\nミニ大会", ["assets/251220_flyer.png"], "cover-top"),
    "tips":     ("技術紹介", "製作に役立つ\n技術情報", ["assets/Sticker.png"], "cover"),
    "showcase": ("ミニかわロボ\n図鑑", "みんなの機体を\n図鑑形式で紹介",
                 ["assets/showcase/004/01.jpg", "assets/showcase/006/01.jpg",
                  "assets/showcase/008/01.jpg", "assets/showcase/005/01.jpg"], "cover"),
    "articles": ("みんなの\n製作記事", "設計思想や\n製作の過程", ["assets/showcase/006/01.jpg"], "cover"),
    "faq":      ("よくある質問", "初心者からの質問と\nその回答", ["assets/Sticker.png"], "cover"),
    "bot":      ("ミニかわBot", "AIが答える\n初心者相談ボット", ["assets/bot.jpg"], "cover"),
    "glossary": ("用語集", "ロボット製作・競技の\n用語を解説", ["assets/Sticker.png"], "cover"),
    "minileopard": ("MiniLeopard", "サンプルプログラム\n(PlatformIO / ESP32)", ["assets/showcase/002/01.jpg"], "cover"),
}


def font(size):
    for name in FONT_BOLD:
        path = os.path.join(FONT_DIR, name)
        if os.path.exists(path):
            return ImageFont.truetype(path, size)
    raise SystemExit("日本語の太字フォントが見つかりません: " + ", ".join(FONT_BOLD))


def fit(im, w, h, mode):
    """im を w x h の枠に合わせた画像を返す。"""
    im = im.convert("RGB")
    if mode == "contain":
        canvas = Image.new("RGB", (w, h), "white")
        s = min(w / im.width, h / im.height)
        r = im.resize((max(1, round(im.width * s)), max(1, round(im.height * s))), Image.LANCZOS)
        canvas.paste(r, ((w - r.width) // 2, (h - r.height) // 2))
        return canvas
    s = max(w / im.width, h / im.height)
    r = im.resize((max(w, round(im.width * s)), max(h, round(im.height * s))), Image.LANCZOS)
    x = (r.width - w) // 2
    y = 0 if mode == "cover-top" else (r.height - h) // 2
    return r.crop((x, y, x + w, y + h))


def fit_font(draw, text, max_w, start, floor=40):
    """text (改行可) が max_w に収まる最大のフォントを返す。"""
    size = start
    while size > floor:
        f = font(size)
        if max(draw.textlength(line, font=f) for line in text.split("\n")) <= max_w:
            return f
        size -= 4
    return font(floor)


def make(slug, title, sub, images, mode):
    card = Image.new("RGB", (W, H), YELLOW)
    d = ImageDraw.Draw(card)

    # 右: ページの画像 (複数なら2列に並べる)
    rw = W - LEFT
    cols = 1 if len(images) == 1 else 2
    rows = -(-len(images) // cols)
    for i, rel in enumerate(images):
        c, r = i % cols, i // cols
        x0, x1 = LEFT + rw * c // cols, LEFT + rw * (c + 1) // cols
        y0, y1 = H * r // rows, H * (r + 1) // rows
        card.paste(fit(Image.open(os.path.join(ROOT, rel)), x1 - x0, y1 - y0, mode), (x0, y0))
    for c in range(1, cols):
        x = LEFT + rw * c // cols
        d.rectangle([x - 3, 0, x + 2, H], fill="white")
    for r in range(1, rows):
        y = H * r // rows
        d.rectangle([LEFT, y - 3, W, y + 2], fill="white")
    d.rectangle([LEFT - 10, 0, LEFT, H], fill=ORANGE)

    # 左: サイト名の帯 + 見出し + 小見出し
    pad = 44
    band = font(30)
    d.rounded_rectangle([pad, 44, LEFT - 10 - pad, 104], radius=30, fill=INK)
    d.text(((LEFT - 10) / 2, 74), "ミニかわロボ公式", font=band, fill=YELLOW, anchor="mm")

    tf = fit_font(d, title, LEFT - 10 - pad * 2, 96)
    d.multiline_text((pad, 150), title, font=tf, fill=INK, spacing=14)
    tb = d.multiline_textbbox((pad, 150), title, font=tf, spacing=14)

    sf = fit_font(d, sub, LEFT - 10 - pad * 2, 38, floor=26)
    d.multiline_text((pad, tb[3] + 40), sub, font=sf, fill=(110, 70, 10), spacing=10)

    d.text((pad, H - 44), "sin1.studio/MiniKawaRobo", font=font(24), fill=ORANGE, anchor="ls")

    os.makedirs(OUT, exist_ok=True)
    card.save(os.path.join(OUT, slug + ".jpg"), quality=88, optimize=True)


if __name__ == "__main__":
    for slug, (title, sub, images, mode) in PAGES.items():
        make(slug, title, sub, images, mode)
        print("ok", slug)
