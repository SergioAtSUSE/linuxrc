/*
 *
 * text.h        Header file for text.c
 *
 * Copyright (c) 1996-2001  Hubert Mantel, SuSE GmbH  (mantel@suse.de)
 *
 */

enum textid_t
    {
    TXT_OK,
    TXT_CANCEL,
    TXT_YES,
    TXT_NO,
    TXT_CHOOSE_LANGUAGE,
    TXT_COLOR_DISPLAY,
    TXT_MONO_DISPLAY,
    TXT_CHOOSE_DISPLAY,
    TXT_MENU_LANG,
    TXT_MENU_MODULES,
    TXT_MENU_START,
    TXT_MENU_INFO,
    TXT_HDR_MAIN,
    TXT_SETTINGS,
    TXT_MENU_DISPLAY,
    TXT_MENU_KEYMAP,
    TXT_LOAD_SCSI,
    TXT_LOAD_CDROM,
    TXT_LOAD_NET,
    TXT_SHOW_MODULES,
    TXT_AUTO_LOAD,
    TXT_DEL_MODULES,
    TXT_TRY_TO_LOAD,
    TXT_LOAD_SUCCESSFUL,
    TXT_LOAD_FAILED,
    TXT_CDROM,
    TXT_FLOPPY,
    TXT_NFS,
    TXT_FTP,
    TXT_START_INSTALL,
    TXT_START_RESCUE,
    TXT_LOAD_RS_NFS,
    TXT_BOOT_SYSTEM,
    TXT_SEARCH_FOR_RI,
    TXT_RI_NOT_FOUND,
    TXT_LOADING,
    TXT_INFO_KERNEL,
    TXT_INFO_CPU,
    TXT_INFO_DEVICES,
    TXT_INFO_DMA,
    TXT_INFO_IRQS,
    TXT_INFO_IOPORTS,
    TXT_INFO_MEM,
    TXT_INFO_MODULES,
    TXT_INFO_PCI,
    TXT_INSERT_DISK,
    TXT_ERROR_READ_DISK,
    TXT_ENTER_ROOT_FS,
    TXT_INVALID_ROOT_FS,
    TXT_INVALID_INPUT,
    TXT_INPUT_IPADDR,
    TXT_INPUT_NETMASK,
    TXT_INPUT_GATEWAY,
    TXT_INPUT_NAMED,
    TXT_FOUND_HOST,
    TXT_ERROR_CONF_NET,
    TXT_PCMCIA_SUCCESS,
    TXT_INPUT_SERVER,
    TXT_INPUT_FTPSERVER,
    TXT_INPUT_DIR,
    TXT_ENTER_PARAMS,
    TXT_TRY_NFS_MOUNT,
    TXT_ERROR_NFSMOUNT,
    TXT_CHOOSE_KEYMAP,
    TXT_DELETE_MODULE,
    TXT_NO_MODULES,
    TXT_AUTO_RUNNING,
    TXT_WANT_MORE_SCSI,
    TXT_WANT_MORE_NET,
    TXT_WANT_MORE_CDROM,
    TXT_NET_ETH0,
    TXT_NET_PLIP,
    TXT_NET_TR0,
    TXT_CHOOSE_NET,
    TXT_INPUT_PLIP_IP,
    TXT_TRY_CD_MOUNT,
    TXT_NET_ARC0,
    TXT_NET_FDDI,
    TXT_NET_HIPPI,
    TXT_INFO_NETDEV,
    TXT_LOAD_PCMCIA,
    TXT_NO_PCMCIA,
    TXT_FOUND_PCMCIA,
    TXT_PCMCIA_PARAMS,
    TXT_START_CARDMGR,
    TXT_PCMCIA_FAIL,
    TXT_PCMCIA_ALREADY,
    TXT_NO_LXRC_END,
    TXT_NO_CTRLC,
    TXT_REMOVE_DISK,
    TXT_EXAMPLE_PAR,
    TXT_HARDDISK,
    TXT_CHOOSE_SOURCE,
    TXT_ERROR_CD_MOUNT,
    TXT_START_YAST,
    TXT_ASK_REBOOT,
    TXT_INSTALL_SUCCESS,
    TXT_ENTER_PARTITION,
    TXT_ERROR_HD_MOUNT,
    TXT_ENTER_HD_DIR,
    TXT_ERROR_INSTALL,
    TXT_DRIVES,
    TXT_LITTLE_MEM,
    TXT_ASK_ANIMATE,
    TXT_ASK_EXPLODE,
    TXT_NO_DRIVES,
    TXT_MENU_EXPERT,
    TXT_FORCE_ROOTIMAGE,
    TXT_NEW_ROOTIMAGE,
    TXT_ASK_RI_FORCE,
    TXT_ENTER_ROOTIMAGE,
    TXT_NOT_IMPLEMENTED,
    TXT_NEW_INST_SYS,
    TXT_ENTER_INST_SYS,
    TXT_NO_INFO_AVAIL,
    TXT_PREPARE_INST,
    TXT_INSERT_LIVECD,
    TXT_START_DEMO,
    TXT_END_REBOOT,
    TXT_ASK_BOOTP,
    TXT_SEND_BOOTP,
    TXT_ERROR_BOOTP,
    TXT_SEARCH_INFOFILE,
    TXT_NFSPORT,
    TXT_ASKING_NAMED,
    TXT_HOST_NOT_FOUND,
    TXT_NET_CONFIGURED,
    TXT_ASK_BREAK,
    TXT_NO_NETDEVICE,
    TXT_ALREADY_FOUND,
    TXT_SCSI_ADAPTER,
    TXT_ANOTHER_MOD,
    TXT_ERROR_FTP,
    TXT_TRY_REACH_FTP,
    TXT_ENTER_FTPUSER,
    TXT_ENTER_FTPPASSWD,
    TXT_ENTER_FTPPROXY,
    TXT_WANT_FTPPROXY,
    TXT_ENTER_FTPPORT,
    TXT_NOMEM_FTP,
    TXT_ANONYM_FTP,
    TXT_ENTER_MODDISK,
    TXT_MORE_MODULES,
    TXT_INIT_PARPORT,
    TXT_ENTER_NFSPORT,
    TXT_YAST1,
    TXT_YAST2,
    TXT_CHOOSE_YAST,
    TXT_INSERT_CD,
    TXT_EJECT_CD,
    TXT_BOOTP_TIMEOUT,
    TXT_ENTER_BOOTP_TIMEOUT,
    TXT_DO_REBOOT,
    TXT_REISERFS
    };

extern char *txt_get  (enum textid_t text_id);
extern int   txt_init (void);
