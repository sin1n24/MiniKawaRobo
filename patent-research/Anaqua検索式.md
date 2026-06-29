# Anaqua（AcclaimIP）用 先行技術 検索式集

調査日: 2026-06-29
対象発明: 同一ピン配の互換マイコンモジュール（カメラ版／液晶版）を入れ替えることで、システムを「遠隔操縦ロボット」⇔「自顔アバターロボット」へ相互転換し、共通ファームが役割を自動判別する構成。

> **構文の前提**: Anaqua の特許検索は AcclaimIP（Lucene 系）を採用しています。本書は以下を前提に記述しています。
> - フィールド: `ttl`(タイトル) / `abst`(要約) / `aclm`(全請求項) / `text`(全文：タイトル+要約+請求項+明細) / `cpc` / `icl`(IPC) / `an`(出願人)
> - 近接: `"word1 word2"~N`（N語以内）／ 論理: `AND OR NOT`／ グループ化: `( )`／ ワイルドカード: `*` `?`
> - お使いの Anaqua のバージョン・検索モジュールにより演算子が異なる場合は、巻末「他構文への読み替え」を参照して調整してください。

---

## 1. 概念ブロック（キーワード）

| # | 概念 | 英語キーワード | 日本語キーワード |
| :- | :--- | :--- | :--- |
| A | 互換・共通・交換可能なマイコンモジュール | modul*, interchangeab*, swap*, plug-in, removable, reconfigur*, "common module", "identical module", microcontroller, microcomputer, MCU, "compute module", "control module" | モジュール, 共通, 交換, 着脱, 差し替え, 兼用, 共用, マイコン, マイクロコントローラ, 制御基板 |
| B | ロボット＋コントローラ（対／送受信） | robot*, controller, "remote control*", teleoperat*, transmitter, receiver, "operation terminal", "operating device" | ロボット, コントローラ, 操縦, 遠隔操作, 送信機, 受信機, 操作端末 |
| C | 役割の自動判別・共通ファーム自己構成 | (automatic* OR self) + (detect* OR determin* OR identif* OR recogni* OR configur*), role, mode, function, firmware, "single program", "common program" | 自動, 判別, 判定, 識別, 認識, 自己構成, 役割, モード, ファームウェア, 共通プログラム |
| D | アバター／顔表示／テレプレゼンス | "avatar robot", telepresence, tele-presence, (operator OR user) + (face OR facial) + (display* OR screen OR monitor) | アバター, テレプレゼンス, 操縦者, 顔, 表示, 化身 |
| E | カメラ⇔ディスプレイ（周辺の入替） | camera, "image sensor", imaging, display, screen, LCD, monitor | カメラ, 撮像, 画像センサ, ディスプレイ, 液晶, 画面 |

---

## 2. すぐ貼れる検索式（AcclaimIP/Lucene 構文）

### 検索1 — 核心：共通モジュールの役割自動判別（最重要）
```
text:(
  (robot* AND (controller OR "remote control" OR teleoperat* OR transmitter OR receiver))
  AND (modul* OR interchangeab* OR swap* OR "plug-in" OR removable OR reconfigur* OR microcontroller* OR "control module")
  AND ((automatic* OR self) AND (detect* OR determin* OR identif* OR recogni* OR configur*))
  AND (role OR mode OR function OR firmware OR "common program" OR "single program")
)
```

### 検索2 — 核心：モジュール入替によるアバター／顔表示への転換
```
text:(
  ("avatar robot" OR telepresence OR tele-presence
     OR "operator's face" OR ("operator" AND "face" AND (display OR screen OR monitor)))
  AND (modul* OR swap* OR interchangeab* OR removable OR "plug-in")
  AND (camera OR "image sensor" OR imaging)
  AND (display OR screen OR LCD OR monitor)
)
```

### 検索3 — 近接演算子で精度を上げた版（役割自動判別）
```
ttl:(robot* OR controller OR teleoperat*) 
AND abst:("automatically determine"~3 OR "automatic detection"~3 OR "self configur*"~2 OR "role"~5)
AND text:(modul* OR interchangeab* OR swap*)
AND text:(camera AND (display OR screen))
```

### 検索4 — 送信機／受信機が同一構成・役割反転（対称ホットスワップの観点）
```
text:(
  ((transmitter AND receiver) OR (controller AND robot))
  AND ("same configuration"~3 OR "identical"~3 OR interchangeab* OR "common hardware"~3 OR swap*)
  AND (role OR mode OR function)
  AND (switch* OR revers* OR interchang*)
)
```

### 検索5 — 分類限定 × キーワード（ノイズ削減・推奨ベース）
```
cpc:(B25J11* OR B25J13* OR B25J9/16* OR H04N7/14* OR A63H30*)
AND text:(
  (modul* OR interchangeab* OR swap* OR "plug-in")
  AND (camera AND (display OR screen))
  AND (robot* AND (controller OR teleoperat*))
)
```

### 検索6 — 日本語公報向け（Anaqua の日本語全文インデックス利用時）
```
text:(
  (ロボット AND (コントローラ OR 操縦 OR 遠隔操作 OR 送信機 OR 受信機))
  AND (モジュール OR 交換 OR 着脱 OR 差し替え OR 兼用 OR 共用 OR マイコン)
  AND (自動 AND (判別 OR 判定 OR 識別 OR 認識))
  AND (カメラ AND (表示 OR 液晶 OR 画面 OR ディスプレイ))
)
```

---

## 3. 分類コード候補（CPC / IPC / FI・Fターム）

検索の起点・絞り込みに有効な分類です。Anaqua では `cpc:` `icl:` で指定します。

### CPC / IPC
- **B25J 9/16** ロボットのプログラム制御
- **B25J 9/0084** モジュール／再構成可能なロボット構成
- **B25J 11/00 (11/0005)** 娯楽・サービス用途のロボット（アバター系）
- **B25J 13/00 (13/006)** ロボットの制御／無線による操縦
- **B25J 5/00** 移動ロボット
- **G05D 1/00** 移動体の遠隔操縦・位置制御
- **H04N 7/14 / 7/142 / 7/147** テレビ電話・双方向映像（操縦者の顔表示）
- **H04N 7/15** テレビ会議システム
- **G06T 13/40** 3D アバターのアニメーション（自顔アバター生成）
- **A63H 30/00 / 30/04** 玩具の無線リモコン（ホビーロボット文脈）
- **G06F 9/445 / G06F 13/40** プログラムのロード／周辺機器接続（プラグアンドプレイによる役割判別の観点）

### 日本特許向け FI / Fターム（J-PlatPat 併用時の参考）
- **FI: B25J13/00** ／ **B25J11/00** ／ **B25J5/00**
- **Fターム: 3C707**（マニピュレータ・ロボット）配下の「遠隔操作」「教示・制御」テーマ
- **Fターム: 2C150**（玩具）配下の「無線操縦」関連

---

## 4. 検索戦略（運用のコツ）

1. **広→狭の順**：まず検索5（分類×キーワード）でヒット件数を把握し、多すぎる場合は概念 C（自動判別）や E（カメラ＋液晶の両立）を AND して絞る。
2. **概念を1つ外して感度確認**：核心の組合せ（A×B×C×D/E）で 0〜少数件なら、概念を1つずつ外して「どの要素が公知の壁か」を可視化する。
3. **被引用・引用ファミリー展開**：近い文献が1件見つかったら、その前方／後方引用と同一パテントファミリーを Anaqua のサイテーション機能で芋づる式に確認。
4. **出願人ノイズ対策**：産業用ロボット大手（FANUC, YASKAWA, ABB, KUKA 等）のヒットが多い場合、本発明はホビー／コンシューマ寄りなので `NOT an:(...)` で大手を一旦除外して見通しを良くする手も有効。
5. **非特許文献（NPL）**：M5Stack「StackChan」、ANA Avatar XPRIZE 等は公知例として効くため、Anaqua の NPL／文献検索があれば日付付きで押さえる。

---

## 5. 他構文への読み替え（参考）

| 機能 | AcclaimIP/Lucene（本書） | Derwent / Questel Orbit 系 | Espacenet/Google Patents 系 |
| :--- | :--- | :--- | :--- |
| 近接（N語以内） | `"a b"~N` | `a (nD) b` / `a NEAR/N b` | （近接非対応、AND で代替） |
| 前方一致 | `word*` | `word+` / `word*` | `word*` |
| タイトル | `ttl:` | `/TI` | `TI=` |
| 要約 | `abst:` | `/AB` | `AB=` |
| 請求項 | `aclm:` | `/CLMS` | （全文に含む） |
| 分類 | `cpc:` | `/CPC` | `CPC=` |

> 注: 演算子の対応は概略です。実際のフィールドコード・近接演算子は Anaqua / 各データベースの最新ヘルプで確認してください。
