
		youbin service 対応の xpbiff 1.27

YOUBIN service を用いてネットワーク越しに動く xpbiff です。
おまけとして、SUN の AUDIO 機能を使ってメール到着を知らせることも
できるようにしてあります。

★ コンパイル
config.h を編集して、

	% xmkmf
	% make depend
	% make

とします。

YOUBIN service を用いるためには、

#define YOUBIN
#define YoubinBinDir youbinのバイナリがおいてあるパス
#define YOUBIN_DEFAULT_SERVER_HOST "youbind が動いているマシン"

を設定して下さい。

★ 追加されたオプション

1. youbin service を使う時のみ有効なオプション

・-server
	youbind が動いているマシンを指定する。
					リソース名 server
					クラス名   Server

・-pipe
	back ground で動くプログラムを指定する。
					リソース名 pipe
					クラス名   Pipe

・-(no)preserve
	メールが届いた時に、以前のメールのヘッダも表示するかどうか
	を指定する。
					リソース名 preserve
					クラス名   Preserve

2. SUN audio 機能を使う時のみ有効なオプション

・-sndfile
	メールが届いた時に鳴らすオーディオデータのファイル名を指定する。
					リソース名 mailSndFile
					クラス名   MailSndFile

・-volume
	オーディオの音量を指定する。
					リソース名 volume
					クラス名   Volume

★ 無効になったオプション

YOUBIN service を用いる場合は、-polling_time オプションは無効になる。
