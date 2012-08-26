#ifndef _TEST_USB_SATA_H_
#define _TEST_USB_SATA_H_

extern int fn_test_am1808_usb(char usb_num);
extern int fn_get_usb_sdnum(char usb_num,char *buf,char *scsi_sd_str);
extern int fn_test_am1808_mmc(void);
extern int fn_test_am1808_nand(void);

#endif
