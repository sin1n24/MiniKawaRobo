---
layout: default
title: "ミニ大会"
nav_order: 40
image: /assets/ogp/archive.jpg
---

# ミニ大会・イベント

ミニかわロボは、2025年から各地でミニ大会が開催されています。以下は、これまでに開催された大会とイベントの記録です。

## 開催形式について

ミニ大会は、今のところ他の大きなイベントに乗っかる形で併催しています。

+ 例：3DPの集い → イベントの１ブースとして出展してミニかわロボのミニ大会を実施
+ 例：電大杯 → かわロボの電大杯の一部として場所をお借りしてミニかわロボのミニ大会を実施

## 大会実績一覧

{% comment %} 表の中身は _data/tournaments.yml。行を足す・直すときはそちらを編集する {% endcomment %}
| 日時 | イベント名 | 優勝ロボ | 場所 | 参加台数 |
| :--- | :--- | :--- | :--- | :--- |
{% for t in site.data.tournaments -%}
{%- capture t_date -%}{% if t.recent %}**{{ t.date }}**{% else %}{{ t.date }}{% endif %}{%- endcapture -%}
{%- capture t_event -%}{% if t.event_url %}[{{ t.event }}]({{ t.event_url }}){% else %}{{ t.event }}{% endif %}{{ t.suffix }}{%- endcapture -%}
{%- capture t_winner -%}
  {%- if t.winner_id and site.data.showcase[t.winner_id] -%}
    <a href="{{ '/showcase/showcase.html' | relative_url }}#m{{ t.winner_id }}" title="ミニかわロボ図鑑 No.{{ t.winner_id }} を見る">👑 {{ t.winner }}</a>
  {%- elsif t.winner_url -%}
    [{{ t.winner }}]({{ t.winner_url }})
  {%- else -%}
    {{ t.winner }}
  {%- endif -%}
{%- endcapture -%}
{%- capture t_count -%}{% if t.count_url %}[{{ t.count }}]({{ t.count_url }}){% else %}{{ t.count }}{% endif %}{%- endcapture -%}
| <span id="{{ t.id }}" class="mkr-tournament-anchor"></span>{{ t_date }} | {{ t_event }} | {{ t_winner }} | {{ t.place }} | {{ t_count }} |
{% endfor %}
<style>
.mkr-tournament-anchor { scroll-margin-top: 6rem; }
tr:has(.mkr-tournament-anchor:target) td { background: #fff4d6; }
</style>

👑 の付いた優勝ロボは [ミニかわロボ図鑑]({% link showcase/showcase.md %}) に登録されている機体で、クリックすると図鑑の該当ページに移動します。

機体ごとの詳しい戦績は、[ミニかわロボ図鑑]({% link showcase/showcase.md %}) の各カードでも確認できます。

## 今後の開催予定について

最新の開催予定は、以下のチャネルでお知らせしています：

+ **[SNS（X/Twitter）](https://x.com/sin1west)** - リアルタイムで最新情報を配信
+ **[公式ブログ](https://sin1.studio/blog/category/ミニかわロボ/)** - 詳細な開催レポートと次回予告
+ **[GitHub Issues](https://github.com/sin1n24/MiniKawaRobo/issues)** - 開催予定の提案・相談

新しいイベント開催の提案や、既存イベントへの出展希望がございましたら、上記のチャネルでお気軽にお問い合わせください。

## 大会レポートの詳細紹介

過去に開催された大会の様子を詳しくご紹介します。

### 1. 初のミニ大会＠3Dプリンタの集い in 大阪 (2025/09/13)
大阪市中央公会堂という歴史ある場所で、記念すべき第1回目のミニ大会が開催されました。3Dプリンタ愛好家が集まる中、自作フレームを駆使したロボットたちが熱戦を繰り広げました。
→ [記事を読む](https://sin1.studio/blog/2025-09-17-225046/)

### 2. 本格トーナメント開催＠電大杯 (2025/11/01)
かわロボの伝統ある「電大杯」に間借りし、10名という多くの参加者が集まりました。本格的なトーナメント形式で、多様な機構を持つロボットたちがぶつかり合う、非常に盛り上がった大会となりました。
→ [記事を読む](https://sin1.studio/blog/2025-11-10-235043/)

### 3. 2025年を締めくくる熱戦＠立命杯 (2025/12/06)
滋賀県で開催された立命杯にて、2025年最後のミニ大会を実施。6名の参加者による熱いバトルが展開され、1年間の活動を締めくくるにふさわしい充実した内容となりました。
→ [記事を読む](https://sin1.studio/blog/2025-12-14-212524/)

### 4. 地域イベントへの出展＠メイカーズながおかまつり (2026/03/01)
長岡市の「メイカーズながおかまつり」に出展。一般来場者への体験操縦会なども行い、ミニかわロボの楽しさをより広い層に伝える機会となりました。
→ [記事を読む](https://sin1.studio/blog/2026-03-14-004040/)
