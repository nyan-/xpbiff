
		youbin service �б��� xpbiff 1.27

YOUBIN service ���Ѥ��ƥͥåȥ���ۤ���ư�� xpbiff �Ǥ���
���ޤ��Ȥ��ơ�SUN �� AUDIO ��ǽ��Ȥäƥ᡼��������Τ餻�뤳�Ȥ�
�Ǥ���褦�ˤ��Ƥ���ޤ���

�� ����ѥ���
config.h ���Խ����ơ�

	% xmkmf
	% make depend
	% make

�Ȥ��ޤ���

YOUBIN service ���Ѥ��뤿��ˤϡ�

#define YOUBIN
#define YoubinBinDir youbin�ΥХ��ʥ꤬�����Ƥ���ѥ�
#define YOUBIN_DEFAULT_SERVER_HOST "youbind ��ư���Ƥ���ޥ���"

�����ꤷ�Ʋ�������

�� �ɲä��줿���ץ����

1. youbin service ��Ȥ����Τ�ͭ���ʥ��ץ����

��-server
	youbind ��ư���Ƥ���ޥ������ꤹ�롣
					�꥽����̾ server
					���饹̾   Server

��-pipe
	back ground ��ư���ץ�������ꤹ�롣
					�꥽����̾ pipe
					���饹̾   Pipe

��-(no)preserve
	�᡼�뤬�Ϥ������ˡ������Υ᡼��Υإå���ɽ�����뤫�ɤ���
	����ꤹ�롣
					�꥽����̾ preserve
					���饹̾   Preserve

2. SUN audio ��ǽ��Ȥ����Τ�ͭ���ʥ��ץ����

��-sndfile
	�᡼�뤬�Ϥ��������Ĥ餹�����ǥ����ǡ����Υե�����̾����ꤹ�롣
					�꥽����̾ mailSndFile
					���饹̾   MailSndFile

��-volume
	�����ǥ����β��̤���ꤹ�롣
					�꥽����̾ volume
					���饹̾   Volume

�� ̵���ˤʤä����ץ����

YOUBIN service ���Ѥ�����ϡ�-polling_time ���ץ�����̵���ˤʤ롣
