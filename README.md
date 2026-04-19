# ミニかわロボ 公式ドキュメント リポジトリ

ここは、手のひらサイズの格闘ロボット競技『ミニかわロボ』の公式リソース集のソースコード（Markdown）を管理している場所です。

**[実際の公式ウェブサイト（閲覧用）はこちら](https://sin1n24.github.io/MiniKawaRobo/)**

## ウェブサイトについて

このリポジトリは、Jekyll + "Just the Docs" テーマを使用して構築されています。

### コンテンツ構成

- **docs/** - ドキュメント（つくりかた、ルール、技術紹介、FAQ、用語集など）
- **showcase/** - 製作例の紹介
- **assets/** - 画像やアイコンなどのリソース
- **_includes/** - Jekyllのカスタムテンプレート

### ページ一覧

| ページ | 説明 |
| :--- | :--- |
| [はじめに](index.md) | サイトのトップページ |
| [ミニかわロボとは](docs/about.md) | ミニかわロボの概要と基本ルール |
| [つくりかた](docs/howto.md) | ロボット製作に必要な5つの要素の解説 |
| [競技ルール](docs/rule.md) | 詳細な競技ルール |
| [技術紹介](docs/tips.md) | 製作に関する技術情報 |
| [ミニ大会](docs/archive.md) | 大会実績とイベント情報 |
| [製作例の紹介](showcase/showcase.md) | 実際に製作されたロボットの紹介 |
| [よくある質問（FAQ）](docs/faq.md) | 初心者からよくいただく質問と回答 |
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
nav_order: 10
---
```

**重要**: `nav_order` は、ナビゲーションメニューでのページの表示順序を決めます。既存のページと重複しないように設定してください。

### 3. ローカルでプレビュー（オプション）

Jekyllをインストール済みの場合：

```bash
bundle install
bundle exec jekyll serve
```

その後、`http://localhost:4000/MiniKawaRobo/` でプレビューできます。

### 4. Pull Requestを送信

修正内容をコミットして、Pull Requestを送信してください。

```bash
git add .
git commit -m "修正内容の説明"
git push origin your-branch-name
```

## ライセンス

このプロジェクトは [MIT License](LICENSE) の下で公開されています。

## サポート・お問い合わせ

- **GitHub Issues** - バグ報告や機能提案
- **[SNS（X/Twitter）](https://x.com/sin1west)** - 質問や相談
- **[公式ブログ](https://sin1n24.hatenablog.com/archive/category/ミニかわロボ)** - 技術情報やイベント情報

---

© 2026 sin1's studio
