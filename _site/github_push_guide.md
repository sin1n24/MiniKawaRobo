# GitHubへの反映手順ガイド

今回修正・改善したファイルを、ご自身のGitHubリポジトリに反映させるための手順です。

## 1. 既存のリポジトリに上書きする場合

1. **ZIPファイルを解凍**し、中の全てのファイルを、ローカルにあるご自身のGitHubリポジトリのディレクトリにコピー（上書き）してください。
2. **Gitコマンド**で変更をコミットし、プッシュします。

```bash
# 1. 変更されたファイルを全てステージング
git add .

# 2. コミット（メッセージは適宜変更してください）
git commit -m "ウェブサイトの改善：FAQ、用語集の追加、ナビゲーションの整理、フッターの拡充"

# 3. GitHubへプッシュ
git push origin main
```

## 2. 新しくリポジトリを初期化してプッシュする場合

もし、新しいディレクトリで作業を始める場合は、以下の手順を行ってください。

```bash
# 1. ディレクトリに移動
cd /path/to/your/MiniKawaRobo_Improved

# 2. Gitリポジトリを初期化
git init

# 3. リモートリポジトリを追加
git remote add origin https://github.com/sin1n24/MiniKawaRobo.git

# 4. 全てのファイルをステージング
git add .

# 5. コミット
git commit -m "Initial commit with improvements"

# 6. プッシュ（既存の履歴を上書きする場合は -f フラグが必要になることがあります）
git push -u origin main
```

## 主な変更点（再確認）
- **構成変更**: `workshop/` を `showcase/` に名称変更し、関連リンクを全て修正
- **新規追加**: `samplecode/` ディレクトリを追加し、`docs/tips.md` に案内を追記

- **新規作成**: `docs/faq.md` (FAQ), `docs/glossary.md` (用語集)
- **改善**: `index.md` (トップページ構成), `docs/tips.md` (技術紹介の整理), `docs/archive.md` (大会情報の拡充), `_includes/nav_footer_custom.html` (フッターのリンク追加)
- **更新**: `README.md` (コントリビューター向けガイドの追加)

---

以上で、改善内容が反映されます。Jekyll (Just the Docsテーマ) の設定に基づいているため、プッシュ後に GitHub Pages で自動的にビルド・公開されるはずです。
