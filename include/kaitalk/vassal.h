/**
 * vassal.h
 * 
 * Created by Kainet
 *
 * HDL controling
 */

#include <kaitalk/gsocket.h>

/**
 * Check crc of packet
 */
short kaitalk_hdl_check_crc(unsigned char *ptr, unsigned char len);

/**
 * Add crc to packet
 */
void kaitalk_hdl_pack_crc(unsigned char *ptr, unsigned char len);

/**
 * Run command from avalible commands
 */
int kaitalk_vassal_run_action(char *action);