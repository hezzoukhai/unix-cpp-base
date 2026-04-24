#ifndef HPS_CARD_H
#define HPS_CARD_H

typedef struct SCard
{
 char     card_number		[ 22];
 int      card_seq;
 char     client_code		[ 24];
 char     bank_code		[ 6];
 char     branch_code		[ 6];
 char     card_product_code	[ 3];
 char     card_fees_code	[ 3];
 char     basic_card_flag	[ 1];
 char     basic_card_number	[ 22];
 char     vip_level		[ 1];
 char     start_val_date	[ 8];
 char     expiry_date		[ 8];
 char     former_expiry_date	[ 8];
 char     delivery_mode		[ 1];
 char     delivery_flag		[ 1];
 char     delivery_date		[ 8];
 char     limits_indexes	[ 4];
 char     periodicity_code	[ 3];
 char     channel_number	[ 15];

}TSCard;

typedef struct STrackIIRecord
{
 char min_card_range	[22 + 1];
 char max_card_range	[22 + 1];
 int field_separator_offset;
 int country_code_offset;
 int expiry_date_offset;
 int service_code_offset;
 int pvki_offset;
 int pvv_offset;
 int cvv_offset;
 int language_code_offset;
}TSTrckIIRecord;
#endif /** HPS_CARD_H **/
