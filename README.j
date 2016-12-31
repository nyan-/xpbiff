

				xpbiff 1.27


	瞬間芸付きのxbiffです :-)
	メイルが来るとヘッダ部をポップアップして知らせる機能もあります。


★ コンパイルと起動

config.hをお好みに応じて編集して頂いて、

	% xmkmf
	% make depend
	% make

などとしてください。

起動は

	% xpbiff

でいいです。



★ 以前のバージョンとの相違点 

今回は下記の方々の御尽力で色々な機能が増えました。この場を借りて感謝致します。

・白石さん(tshira@mtl.t.u-tokyo.ac.jp)によるShape extensionの実現
・清さん(sei@randd.jba.co.jp)によるMIME decoding (ISO-2022-JP only)の実現
・松下さん(matusita@ics.es.osaka-u.ac.jp)によるPopupGeometryのBugFix
・山本さん(y-yamamt@ics.es.osaka-u.ac.jp)によるXPM support/Command Line部の
          追加変更。
・倉嶋さん(kura@mmp.cl.nec.co.jp)によるMHユーザーの為の拡張。

★ 今回追加されたオプション

  ・-shape
	shape 機能を使用するかどうかを指定する．
					リソース名 shape
					クラス名   Shape

  ・-arrive_bitmap_mask bitmap_file 
        メイル到着時に表示するピックスマップに対するマスクパタンのビッ
        トマップを指定する．
					リソース名 arrivePixmapMask
					クラス名   ArrivePixmapMask

  ・-no_bitmap_mask bitmap_file
	メイル未到着時に表示するピックスマップに対するマスクパタンのビッ
	トマップを指定する．
					リソース名 nomailPixmapMask
					クラス名   NomailPixmapMask

  ・-arrive_bitmap {bitmap_file|pixmap_file}
	メイル到着時に表示するピックスマップを XBM ファイルまたは XPM 
	ファイルで指定する．
					リソース名 arrivePixmap
					クラス名   ArrivePixmap

  ・-no_bitmap {bitmap_file|pixmap_file}
	メイル未到着時に表示するピックスマップを XBM ファイルまたは 
	XPM ファイルで指定する．
					リソース名 nomailPixmap
					クラス名   NomailPixmap

  ・-prefer prefer_list
	ポップアップウィンドウに表示するメイルヘッダーのフィールド名を
	空白で区切って指定する．フィールド名の先頭に `&' を付けるとそ
	のフィールドがメイルに含まれている場合のみ表示し，`&' を付けな
	ければそのフィールドが含まれていない場合には "(none)" という文
	字列を表示する．（既定値 From Subject）
					リソース名 prefer
					クラス名   Prefer

  ・-mhscan
	ポップアップウィンドウに表示するメイルヘッダーを MH の scan コ
	マンドを使って作成する．
					リソース名 mhscan
					クラス名   Mhscan
  ・-shape
        shape 機能を使用する.
  ・-noshape
        shape 機能を使用しない.
                                        リソース名 shape
                                        クラス名   Shape

  ・-mhscan
        ポップアップウィンドウに表示するメイルヘッダーを MH の scan コ
        マンドを使って作成する．
  ・-nomhscan
        ポップアップウィンドウに表示するメイルヘッダーを MH の scan コ
        マンドを使わずに作成する．
                                        リソース名 mhscan
                                        クラス名   Mhscan

  ・-scan_command command_string
        -mhscan が指定された時、あるいは mhscan リソースがTrueの時だけ
        有効なオプションで、MHのscanコマンドの替わりにスプールファイル
        のフォーマット変換を行うコマンドを指定します。スプールファイル
        参照時は、このコマンドの後にもう一つスプールファイル名がパラメー
        タとして付け加えられますので注意してください。
        (例)
        xpbiff -mhscan -scan_command 'scan -file'
        デフォルトのscanのフォーマットで表示します。grep, sed, awk,
        perl などでスクリプトを書くのもOKです。
                                        リソース名 scanCommand
                                        クラス名   ScanCommand

   ・-scan_command_path command_path
        -scan_command のコマンドのあるディレクトリを指定します。このオ
        プションは -scan_command あるいは scanCommand リソースで指定さ
        れたコマンドが相対パス(`/'で始まらない)場合のみ有効です。現在
        は、複数のディレクトリを設定することはできません。
                                        リソース名 scanCommandPath
                                        クラス名   ScanCommandPath

★ Special Thanks to:

    以下の方々にコメント・助言・パッチ等を送って頂きました
    ありがとうございました。

	Akihisa Kurashima <kura@mmp.cl.nec.co.jp>
	Andrew Krzywdzinski <andrew@cs.utk.edu>
	Andy Malis <malis@BBN.COM>
	Bob Primmer <rprimmer%lemond.webo.dg.com@RELAY.CS.NET>
	Chris Garrigues <7thSon@SLCS.SLB.COM>
	David Asano <david@comm.toronto.edu>
	Felix Ingrand <felix@ai.sri.com>
	Hidetsugu Nakashio <naka@soum.co.jp>
	Kazuyoshi SEI <sei@randd.jba.co.jp> 
	Ken Nelson <nelson@wrl.EPI.COM>
	Luis Miguel Silveira <lms@sobolev.mit.edu>
	MAKOTO Matsushita <matusita@ics.es.osaka-u.ac.jp>
	Michael Glad <glad@daimi.aau.dk>
	Peter Jackson <peterj@cs.adelaide.edu.au>
	Ricardo Cardenas <rcardena@us.oracle.com>
	Robert Lupton (the Good) <rhl@astro.Princeton.EDU>
	Steven Reiz <sreiz@cs.vu.nl>
	Sting Takubon TAKEUCHI <take@train.ncl.omron.co.jp>
	Yoshikazu Yamamoto <y-yamamt@ics.es.osaka-u.ac.jp>
	chiba@is.s.u-tokyo.ac.jp (Chiba)
	hkato@krc.sony.co.jp (Hiroshi KATO)
	hotta@flab.fujitsu.co.jp (Yuji Hotta)
	imp@Solbourne.COM (Warner Losh)
	jgraf@smdgr1.intel.com (Jim Graf )
	lindheim@sampson.ccsf.caltech.edu (Jan Lindheim)
	lusk@antares.mcs.anl.gov (Rusty Lusk)
	masuhara@is.s.u-tokyo.ac.jp
	murase@ctf5.drl.mei.co.jp (Kouichi Murase)
	nishida@phantom.src.ricoh.co.jp (Akihiro Nishida)
	rekers@cwi.nl (Jan Rekers)
	remy@cs.uoregon.edu (Remy Evard)
	saka@pfu.fujitsu.co.JP (SAKAEDA Ken-ichi)
	sako@joke.cl.nec.co.jp (SAKO Shinji)
	site@mfd.cs.fujitsu.co.jp (SAITOH-Ely-Yasuhiro)
	tshira@mtl.t.u-tokyo.ac.jp
	vita@oxford.dab.ge.com (Mark Vita)

  						THANKS A LOT!!!!

-------------------------------------------------------------------------------
					首藤 一彦
					ヤマハ株式会社 電子楽器事業本部 
                                        ＣＢＸ開発室
					shutoh@lm.emi.yamaha.co.jp
