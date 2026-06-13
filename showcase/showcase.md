---
layout: default
title: "ミニかわロボ図鑑"
nav_order: 70
---

# ミニかわロボ図鑑

これまでに製作されたミニかわロボの機体を、図鑑形式で紹介します。公式キットをベースにした作例から、独自の工夫を凝らしたカスタム機まで、各機体の構成や特徴をまとめています。

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

製作の詳しい過程やストーリーは、[みんなの製作記事]({% link showcase/articles.md %}) もあわせてご覧ください。

---

## 製作例の投稿について
現在、公式サイトへの投稿フォームを準備中です。ご自身の製作したロボットを掲載希望の方は、SNS（X/Twitter）でハッシュタグ **#ミニかわロボ** を付けて投稿いただくか、[GitHub Issues](https://github.com/sin1n24/MiniKawaRobo/issues)でご紹介ください！
