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
.showcase-imgwrap { position: relative; }
.showcase-img {
  width: 100%;
  aspect-ratio: 4 / 3;
  object-fit: cover;
  display: block;
  background: #f5f5f5;
}
.showcase-no {
  position: absolute;
  top: .5rem;
  left: .5rem;
  background: rgba(0,0,0,.66);
  color: #fff;
  font-size: .78rem;
  font-weight: 700;
  letter-spacing: .04em;
  padding: .15rem .55rem;
  border-radius: 999px;
}
.showcase-body { padding: 1rem 1.1rem 1.2rem; }
.showcase-name { margin: 0 0 .2rem; font-size: 1.15rem; font-weight: 700; line-height: 1.3; }
.showcase-creator { margin: 0 0 .6rem; color: #666; font-size: .85rem; }
.showcase-desc p { margin: .4rem 0; }
.showcase-desc ul { margin: .4rem 0 0; padding-left: 1.2rem; }
.showcase-desc li { margin: .15rem 0; }
.showcase-video a { font-weight: 600; }
.showcase-results { margin-top: .7rem; padding-top: .6rem; border-top: 1px dashed #e0e0e0; }
.showcase-badge {
  display: inline-block;
  background: #fff4d6;
  color: #8a6100;
  font-size: .78rem;
  font-weight: 700;
  padding: .15rem .55rem;
  border-radius: 999px;
  margin-bottom: .4rem;
}
.showcase-result-list { margin: 0; padding-left: 1.1rem; font-size: .82rem; color: #555; }
.showcase-result-list li { margin: .15rem 0; }
@keyframes showcaseFadeIn {
  from { opacity: 0; transform: translateY(12px); }
  to   { opacity: 1; transform: none; }
}
@media (prefers-reduced-motion: reduce) {
  .showcase-card { animation: none; }
}
</style>

{% assign entries = site.data.showcase | sort %}
{% assign result_list = "" | split: "," %}
{% if site.data.results %}
  {% assign result_list = site.data.results | sort %}
{% endif %}
<div class="showcase-grid">
{% for item in entries %}
  {% assign m = item[1] %}
  {% assign m_results = result_list | where_exp: "r", "r[1].machine_id == m.id" %}
  {% assign win_count = m_results | where_exp: "r", "r[1].result_rank == 1" | size %}
  <div class="showcase-card">
    <div class="showcase-imgwrap">
      {% if m.images and m.images.size > 0 %}
      <img class="showcase-img" src="{{ m.images[0] | prepend: '/' | relative_url }}" alt="{{ m.name }}" loading="lazy">
      {% endif %}
      <span class="showcase-no">No.{{ m.id }}</span>
    </div>
    <div class="showcase-body">
      <div class="showcase-name">{{ m.name }}</div>
      {% if m.creator and m.creator != "" %}<p class="showcase-creator">製作: {{ m.creator }}</p>{% endif %}
      <div class="showcase-desc">{{ m.description | markdownify }}</div>
      {% if m.video_url and m.video_url != "" %}
      <p class="showcase-video"><a href="{{ m.video_url }}" target="_blank" rel="noopener">▶ 動画を見る</a></p>
      {% endif %}
      {% if m_results.size > 0 %}
      <div class="showcase-results">
        {% if win_count > 0 %}<span class="showcase-badge">🏆 優勝 {{ win_count }}回</span>{% endif %}
        <ul class="showcase-result-list">
          {% for ritem in m_results reversed limit:3 %}
            {% assign r = ritem[1] %}
            <li>{{ r.event_date }}｜{{ r.event_name }}｜{{ r.result }}{% if r.rounds and r.rounds != "" %}（{{ r.rounds }}）{% endif %}</li>
          {% endfor %}
        </ul>
      </div>
      {% endif %}
    </div>
  </div>
{% endfor %}
</div>

製作の詳しい過程やストーリーは、[みんなの製作記事]({% link showcase/articles.md %}) もあわせてご覧ください。大会全体の記録は [ミニ大会ページ]({% link docs/archive.md %}) もあわせてご覧ください。

---

## 図鑑への登録について
ご自身が製作したミニかわロボを、この図鑑に掲載してみませんか？ **[製作例 登録フォーム](https://forms.gle/xwG7DzkVWiRUbbFR8)** から、機体名・製作者・説明・写真（JPGまたはPNG）などを送信するだけで登録できます（Googleアカウントでのログインが必要です）。

図鑑に登録済みの機体で大会に参加された方は、同じフォームの「登録の種類」で **大会結果の追加** を選ぶことで、機体情報とは別に大会結果だけを追加投稿できます。

このほか、SNS（X/Twitter）でハッシュタグ **#ミニかわロボ** を付けて投稿いただくか、[GitHub Issues](https://github.com/sin1n24/MiniKawaRobo/issues) でのご紹介も歓迎しています！
