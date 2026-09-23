# pebble-mimikko

Pebble 2 Duo（flint）向けウォッチフェイス。

時計・日付と一緒に、関西弁のひとことメッセージをランダム表示する「一方通行のペット時計」。
お世話は不要、ただ見て時々つぶやいてくれるだけ。

## 見た目

- 黒背景・白文字
- 上から日付（例: SEP 23）／時計（24時間表示）／メッセージ
- メッセージは15分おきに自動で切り替わる（MINUTE_UNIT tick、SECOND_UNIT不使用）
- 手首を振る（タップ）と即座にメッセージが切り替わる

## メッセージ

全150個、ひらがな・カタカナのみ（漢字非対応の自作サブセットフォント）。

- メンヘラ系 40個
- デレ系 60個
- 食べ物系 50個

## 開発環境

Pebble SDKはWindows非対応のため、WSL2上で開発する。

```bash
# WSL2 Ubuntu内
uv tool install pebble-tool
pebble build
pebble install --emulator flint --vnc   # ヘッドレス環境なので --vnc 必須
pebble install --phone <iPhoneのIP>      # 実機転送（Pebbleアプリの開発者接続を使用）
```

## 対応機種

flint（Pebble 2 Duo）専用。
