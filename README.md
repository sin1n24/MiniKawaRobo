# ミニかわロボ 公式ドキュメント リポジトリ

ここは、手のひらサイズの格闘ロボット競技『ミニかわロボ』の公式リソース集のソースコード（Markdown）を管理している場所です。

**[実際の公式ウェブサイト（閲覧用）はこちら](https://sin1.studio/MiniKawaRobo/)**

## ウェブサイトについて

このリポジトリは、Jekyll + "Just the Docs" テーマを使用して構築されています。

### コンテンツ構成

- **docs/** - ドキュメント（つくりかた、ルール、技術紹介、FAQ、用語集、ワークショップなど）
- **showcase/** - ミニかわロボ図鑑・みんなの製作記事
- **samplecode/** - サンプルプログラムの保管場所
- **assets/** - 画像やアイコンなどのリソース（`assets/ogp/` はSNS共有用のOGP画像、`assets/showcase/` は図鑑の機体写真）
- **_data/** - ページの元データ（`tournaments.yml` 大会実績、`showcase/` 図鑑の機体、`results/` 機体ごとの大会結果）
- **_includes/** / **_layouts/** - Jekyllのカスタムテンプレート
- **_tools/** - メンテナンス用スクリプト（サイトには公開されません）

### ページ一覧

| ページ | 説明 |
| :--- | :--- |
| [はじめに](index.md) | サイトのトップページ |
| [ミニかわロボとは](docs/about.md) | ミニかわロボの概要と基本ルール |
| [つくりかた](docs/howto.md) | ロボット製作に必要な5つの要素の解説 |
| [競技ルール](docs/rule.md) | 詳細な競技ルール |
| [技術紹介](docs/tips.md) | 製作に関する技術情報 |
| [ミニ大会](docs/archive.md) | 大会実績とイベント情報 |
| [ミニかわロボ図鑑](showcase/showcase.md) | これまでに製作された機体を図鑑形式で紹介 |
| [みんなの製作記事](showcase/articles.md) | ユーザーによるミニかわロボの製作記事 |
| [ワークショップ](docs/workshop.md) | 過去の開催実績と今後のワークショップ情報 |
| [よくある質問（FAQ）](docs/faq.md) | 初心者からよくいただく質問と回答 |
| [ミニかわBot](docs/bot.md) | 初心者相談AIボット |
| [用語集](docs/glossary.md) | ロボット製作に関連する専門用語の解説 |

## 共同開発者・コントリビューターの方へ

サイトの修正、ルールの更新、新しいコンテンツの追加を提案する場合は、以下の手順でお願いします。

### 1. リポジトリをFork

```bash
git clone https://github.com/YOUR_USERNAME/MiniKawaRobo.git
cd MiniKawaRobo
```

### 2. 修正内容を作成

Markdownファイルを編集します。新しいページを追加する場合は、以下のフロントマターを含めてください：

```markdown
---
layout: default
title: "ページタイトル"
nav_order: 100
---
```

**重要**: `nav_order` は、ナビゲーションメニューでのページの表示順序を決めます。既存のページと重複しないように設定してください。

### 3. ローカルでプレビュー（オプション）

Jekyllをインストール済みの場合（Gemfileは無いので `jekyll` を直接実行します。`jekyll-remote-theme` と `jekyll-include-cache` のgemが必要です）：

```bash
jekyll serve
```

その後、`http://localhost:4000/MiniKawaRobo/` でプレビューできます。

### 4. Pull Requestを送信

修正内容をコミットして、Pull Requestを送信してください。

```bash
git add .
git commit -m "修正内容の説明"
git push origin your-branch-name
```

## メンテナー向け情報

日々の更新でよく触る場所と、はまりやすい点をまとめています。`main` にpushすると、1〜2分でGitHub Pagesに反映されます。

### 大会実績を追加・修正する

[ミニ大会](docs/archive.md) の「大会実績一覧」の表は `_data/tournaments.yml` から自動生成しています。`docs/archive.md` の表を直接書き換えるのではなく、このファイルを編集してください。

```yaml
- id: t20261031            # 行のアンカー名。他と重複させない（t + 開催日）
  date: "2026/10/31"
  recent: true             # 日付を太字にする（任意）
  event: 電大杯
  event_url: https://...   # イベント名をリンクにする（任意）
  suffix: "-day1"          # イベント名の後ろに付ける文字（任意）
  winner: ぷち矛くん        # 表に出す優勝ロボ名
  winner_url: https://...  # 優勝ロボ欄をリンクにする（任意。winner_id があるときは不要）
  winner_id: "009"         # 優勝ロボが図鑑にある場合、その機体番号（任意）
  place: 東京 北千住
  count: 10                # 参加台数
  count_url: https://...   # 参加台数欄をリンクにする（任意）
```

### 図鑑の機体に王冠マークを付ける

優勝ロボが [ミニかわロボ図鑑](showcase/showcase.md) に登録されている場合、その大会のブロックに `winner_id` を1行足します。

- 図鑑の機体写真の左下に 👑 が付き、クリックするとミニ大会ページの該当行へ移動します（複数回優勝している機体は、表でいちばん下＝新しい大会の行へ飛びます）。
- ミニ大会ページの優勝ロボ欄も、図鑑の該当カードへの 👑 付きリンクになります。
- **番号は必ず `"009"` のように引用符付きの3桁で書きます。** 引用符が無いと数値として読まれ、一致しなくなります。
- `winner` は表示名なので、図鑑の機体名と違っていて構いません（例: 表では「ミニレパードS」、図鑑では「ミニレパード青」）。
- 外すときは `winner_id` の行を消すだけです。

### 図鑑の機体登録の流れ

図鑑のデータは手で書かず、[登録フォーム](https://forms.gle/xwG7DzkVWiRUbbFR8)（Google Forms）→ Google Apps Script → このリポジトリへの自動コミット、という流れで入ります。

- `_data/showcase/{番号}.yml` … 機体情報。上書き登録のたびに丸ごと再生成されるので、手で足した項目は消えます。
- `assets/showcase/{番号}/NN.jpg` … 機体写真。
- `showcase/robots/{番号}.md` … 機体ごとの「XでPOST」用の共有ページ（機体写真をOGP画像にして、図鑑の該当カードへ転送するだけのページ）。
- `_data/results/*.yml` … フォームの「大会結果の追加」で投稿された機体ごとの戦績。
- `_showcase_history/` … 上書き前のバックアップ。

### OGP画像（SNSで共有したときのカード画像）

各ページの front matter の `image:` でOGP画像を指定しています。指定が無いページは `_config.yml` の `defaults` にある既定画像になります。

ページを追加したときは、次の手順で同じデザインの画像を作れます（Python と Pillow が必要。フォントはWindowsの日本語フォントを使います）。

1. `_tools/make_ogp.py` の `PAGES` に1行追加（見出し・小見出し・右側に置く画像）
2. リポジトリ直下で `python _tools/make_ogp.py` を実行 → `assets/ogp/<slug>.jpg` ができる
3. そのページの front matter に `image: /assets/ogp/<slug>.jpg` を追加

Xは一度読んだカード画像をしばらくキャッシュするので、差し替えてもすぐには切り替わらないことがあります。

### 「XでPOST」ボタン

全ページ共通のボタンは `_includes/xpost_button.html` で、`header_custom.html`（上部バー）と `footer_custom.html`（ページ末尾）から読み込んでいます。投稿文を変えるときは `xpost_button.html` を、見た目を変えるときは `_includes/head_custom.html` の `.mkr-xpost` を編集します。図鑑の機体ごとのボタンは `showcase/showcase.md` 側にあります。

### はまりやすい点

- **インラインの `<script>` 内に `//` の行コメントを書かない。** HTMLが1行に圧縮されるため、以降のコードがすべてコメントになって動かなくなります（`/* */` を使う）。
- **図鑑のカードに `data-aos` を付けない。** スクロール検知が効かず、透明のまま表示されなくなります。
- **機体番号の照合に Liquid の `where` フィルタを使わない。** `"005"` が数値扱いされて一致しません（`where_exp` を使う）。
- **`site.data.xxx | sort` の結果に `reverse` フィルタを使わない。** 中身が崩れます（forタグの `reversed` 指定を使う）。

## ライセンス

このプロジェクトは [MIT License](LICENSE) の下で公開されています。

## サポート・お問い合わせ

- **GitHub Issues** - バグ報告や機能提案
- **[SNS（X/Twitter）](https://x.com/sin1west)** - 質問や相談
- **[公式ブログ](https://sin1.studio/blog/category/ミニかわロボ/)** - 技術情報やイベント情報

---

© 2026 sin1's studio
