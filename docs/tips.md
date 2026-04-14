---
layout: default
title: "技術紹介"
nav_order: 4
---

# 技術紹介

ミニかわロボの製作に関する技術情報を、初心者向けから上級者向けまで幅広く紹介しています。以下のコンテンツをご参照ください。

1. TOC
{:toc}

---

## サンプルプログラムの提供について

本プロジェクトでは、リポジトリ内の **`samplecode/`** ディレクトリに、ミニかわロボを動かすためのサンプルプログラムを順次公開していく予定です。

- **[samplecode/ ディレクトリを見る](https://github.com/sin1n24/MiniKawaRobo/tree/main/samplecode)**
  - ※GitHubのリポジトリを直接参照してください。
  - 今後、M5Atom S3やM5Capsule向けの基本的な動作コードや、コントローラー連携用のコードを追加予定です。

---

## 初心者向けガイド

初めてミニかわロボを製作される方は、まず以下の記事をご覧ください。基本的な構成と製作方法が理解できます。

### M5シリーズを用いてミニかわロボを動かす（初心者向け）

M5Atom S3やM5Capsuleを使用した基本的な構成と、スマートフォンアプリでの操縦方法を解説しています。プログラミングの知識がなくても、この記事に従うことで動作するロボットが完成します。

**主な内容:**
- M5ボードの選択と基本設定
- サーボの接続方法
- スマートフォンアプリ（UGOKU Pad、Blynk IoTなど）を使用した操縦
- 簡単なトラブルシューティング

<iframe class="note-embed" src="https://note.com/embed/notes/n0a02cd8ad297" style="border: 0; display: block; max-width: 99%; width: 494px; padding: 0px; margin: 10px 0px; position: static; visibility: visible;" height="400"></iframe><script async src="https://note.com/scripts/embed.js" charset="utf-8"></script>

---

## 中級者向けガイド

基本的な構成を理解した後、より高度な機能や最適化を学びたい方向けのコンテンツです。

### M5ATOMを用いてミニかわロボを動かすコード（上級者向け）

M5Atom S3を使用した、より詳細なプログラミング例を紹介しています。カスタムコントローラの実装や、複雑な動作シーケンスの設計方法が学べます。

**主な内容:**
- Arduino IDEでのコーディング
- PWM制御によるサーボの精密制御
- 無線通信プロトコル（BLE、WiFi、ESP-NOW）の実装
- センサー連携による自動制御

<iframe class="note-embed" src="https://note.com/embed/notes/nb0adce424a60" style="border: 0; display: block; max-width: 99%; width: 494px; padding: 0px; margin: 10px 0px; position: static; visibility: visible;" height="400"></iframe><script async src="https://note.com/scripts/embed.js" charset="utf-8"></script>

<iframe class="note-embed" src="https://note.com/embed/notes/n4e82001dec61" style="border: 0; display: block; max-width: 99%; width: 494px; padding: 0px; margin: 10px 0px; position: static; visibility: visible;" height="400"></iframe><script async src="https://note.com/scripts/embed.js" charset="utf-8"></script>

---

## 機械設計・最適化

ロボットの性能を向上させるための機械設計に関する情報です。

### M5StickC+/Atom/Capsule向 サーボ接続基板の紹介

マイコンとサーボを効率的に接続するための基板について、設計思想と使用方法を解説しています。

**主な内容:**
- 基板の選択基準
- 配線の最適化
- 電力管理
- トラブルシューティング

<iframe src="https://hatenablog-parts.com/embed?url=https://sin1n24.hatenablog.com/entry/2024/01/03/010304" 
        style="border: 0; display: block; width: 100%; height: 155px; max-width: 500px; margin: 10px 0px;" 
        frameborder="0" scrolling="no"></iframe>

### ダブルヘリカルギア（やまば歯車）のご紹介

複雑な動きを実現するためのギア機構について、設計と実装方法を紹介しています。

**主な内容:**
- ギア比の計算
- 3Dプリンタでの製作方法
- 動作の最適化
- 実装例

<iframe src="https://hatenablog-parts.com/embed?url=https://sin1n24.hatenablog.com/entry/2024/03/12/231830" 
        style="border: 0; display: block; width: 100%; height: 155px; max-width: 500px; margin: 10px 0px;" 
        frameborder="0" scrolling="no"></iframe>

---

## コントローラー設計

操縦系の設計に関する情報です。

### M5ATOM S3向け小型ジョイスティックコントローラについて

スマートフォンアプリではなく、専用のコントローラを自作したい方向けのガイドです。

**主な内容:**
- ジョイスティックの選択と接続
- コントローラ基板の設計
- プログラミング例
- ケースの設計・製作

<iframe src="https://hatenablog-parts.com/embed?url=https://sin1n24.hatenablog.com/entry/2025/01/24/005304" 
        style="border: 0; display: block; width: 100%; height: 155px; max-width: 500px; margin: 10px 0px;" 
        frameborder="0" scrolling="no"></iframe>

---

## 総合的な設計・製作・運用ガイド

ロボット全体の設計から運用まで、総合的なノウハウをまとめた記事です。

### ミニかわロボ設計・製作・操縦・運用のポイント

ミニかわロボの開発者による、総合的な設計・製作・運用ガイドです。実際の大会出場経験に基づいたノウハウが詰まっています。

**主な内容:**
- 全体的な設計思想
- 製作時の注意点
- 操縦技術の向上
- 大会での運用ノウハウ
- トラブル対応

<iframe src="https://hatenablog-parts.com/embed?url=https://sin1n24.hatenablog.com/entry/2023/12/18/000338" 
        style="border: 0; display: block; width: 100%; height: 155px; max-width: 500px; margin: 10px 0px;" 
        frameborder="0" scrolling="no"></iframe>

---

## その他のリソース

### 設計データ

3Dプリンタでフレームを製作する際に使用できる設計データが、以下のプラットフォームで公開されています：

- **[Makerworld](https://makerworld.com/)** - 公式の設計データ
- **[Thingiverse](https://www.thingiverse.com/)** - コミュニティによる設計データ

### ブログ

詳細な技術情報やイベントレポートは、[公式ブログ](https://sin1n24.hatenablog.com/archive/category/ミニかわロボ)で随時更新されています。

### GitHub

ソースコード、設計データ、ドキュメントは、[GitHubリポジトリ](https://github.com/sin1n24/MiniKawaRobo)で公開されています。

---

ご不明な点やご質問がございましたら、[GitHub Issues](https://github.com/sin1n24/MiniKawaRobo/issues)や[SNS](https://x.com/sin1west)でお気軽にお問い合わせください。
