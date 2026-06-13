---
layout: default
title: "製作例の紹介"
nav_order: 70
---

# 製作例の紹介

ミニかわロボの製作例を紹介します。公式のキットや設計データを使用した例から、ユーザーによる独創的なカスタマイズまで、幅広く掲載しています。

1. TOC
{:toc}

---

<style>
.showcase-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
  gap: 1.5rem;
  margin: 1.5rem 0;
}
.showcase-card {
  border: 1px solid #e0e0e0;
  border-radius: 10px;
  overflow: hidden;
  background: #fff;
  box-shadow: 0 2px 6px rgba(0,0,0,.06);
  display: flex;
  flex-direction: column;
  /* 基底は可視。読み込み時に一度だけ再生され、必ず opacity:1 で終わる安全なフェード */
  opacity: 1;
  animation: showcaseFadeIn .5s ease both;
}
.showcase-img {
  width: 100%;
  aspect-ratio: 4 / 3;
  object-fit: cover;
  display: block;
  background: #f5f5f5;
}
.showcase-body { padding: 1rem 1.1rem 1.2rem; }
.showcase-name { margin: 0 0 .2rem; font-size: 1.15rem; font-weight: 700; line-height: 1.3; }
.showcase-creator { margin: 0 0 .6rem; color: #666; font-size: .85rem; }
.showcase-desc p { margin: .4rem 0; }
.showcase-desc ul { margin: .4rem 0 0; padding-left: 1.2rem; }
.showcase-desc li { margin: .15rem 0; }
.showcase-video a { font-weight: 600; }
@keyframes showcaseFadeIn {
  from { opacity: 0; transform: translateY(12px); }
  to   { opacity: 1; transform: none; }
}
@media (prefers-reduced-motion: reduce) {
  .showcase-card { animation: none; }
}
</style>

## 公式の製作例

{% assign entries = site.data.showcase | sort %}
<div class="showcase-grid">
{% for item in entries %}
  {% assign m = item[1] %}
  <div class="showcase-card">
    {% if m.images and m.images.size > 0 %}
    <img class="showcase-img" src="{{ m.images[0] | prepend: '/' | relative_url }}" alt="{{ m.name }}" loading="lazy">
    {% endif %}
    <div class="showcase-body">
      <div class="showcase-name">{{ m.name }}</div>
      {% if m.creator and m.creator != "" %}<p class="showcase-creator">製作: {{ m.creator }}</p>{% endif %}
      <div class="showcase-desc">{{ m.description | markdownify }}</div>
      {% if m.video_url and m.video_url != "" %}
      <p class="showcase-video"><a href="{{ m.video_url }}" target="_blank" rel="noopener">▶ 動画を見る</a></p>
      {% endif %}
    </div>
  </div>
{% endfor %}
</div>

---

## みんなの製作例

ユーザーの皆様による、独創的なミニかわロボの製作例をご紹介します。

### ミニかわロボ作ってみた！｜すーほ。
スナップフィット構造を用いてネジを極力使わず組み立てられる機体「グリモルディ」の紹介として設計思想や車輪の換装ギミックなどを紹介しています。

また、ミニかわロボが「かわロボOBの復帰のきっかけ」や「非競技系メイカーが勝負の楽しさを知る場」、さらには参加者のレベルに応じた（子供から大人まで）初心者向け教材として非常に有効であるとコメント頂きました。

<iframe class="note-embed" src="https://note.com/embed/notes/n0a02cd8ad297" style="border: 0; display: block; max-width: 99%; width: 494px; padding: 0px; margin: 10px 0px; position: static; visibility: visible;" height="400"></iframe><script async src="https://note.com/scripts/embed.js" charset="utf-8"></script>

### ミニかわロボ　初制作機体｜下克上
100円ショップのカービィのフィギュア、端材のポリカーボネート板、輪ゴムなどを活用し、数時間で「サーボスター」という機体を完成させた製作記です
。

激安サーボ特有のトラブルを抱えながらも、試合として十分に成立し、平和でユルい雰囲気で楽しめた旨をコメント頂きました。
<iframe class="note-embed" src="https://note.com/embed/notes/nb0adce424a60" style="border: 0; display: block; max-width: 99%; width: 494px; padding: 0px; margin: 10px 0px; position: static; visibility: visible;" height="400"></iframe><script async src="https://note.com/scripts/embed.js" charset="utf-8"></script>

---

## 製作例の投稿について
現在、公式サイトへの投稿フォームを準備中です。ご自身の製作したロボットを掲載希望の方は、SNS（X/Twitter）でハッシュタグ **#ミニかわロボ** を付けて投稿いただくか、[GitHub Issues](https://github.com/sin1n24/MiniKawaRobo/issues)でご紹介ください！
