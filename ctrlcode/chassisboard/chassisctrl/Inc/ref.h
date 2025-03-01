#ifndef REFEREE_H
#define REFEREE_H

#include "protocol.h"

typedef enum
{
    RED_HERO = 1,
    RED_ENGINEER = 2,
    RED_STANDARD_1 = 3,
    RED_STANDARD_2 = 4,
    RED_STANDARD_3 = 5,
    RED_AERIAL = 6,
    RED_SENTRY = 7,
    BLUE_HERO = 11,
    BLUE_ENGINEER = 12,
    BLUE_STANDARD_1 = 13,
    BLUE_STANDARD_2 = 14,
    BLUE_STANDARD_3 = 15,
    BLUE_AERIAL = 16,
    BLUE_SENTRY = 17,
} robot_id_t;
typedef enum
{
    PROGRESS_UNSTART = 0,
    PROGRESS_PREPARE = 1,
    PROGRESS_SELFCHECK = 2,
    PROGRESS_5sCOUNTDOWN = 3,
    PROGRESS_BATTLE = 4,
    PROGRESS_CALCULATING = 5,
} game_progress_t;

typedef struct // 0001
{
    uint8_t game_type : 4;
    uint8_t game_progress : 4;
    uint16_t stage_remain_time;
    uint64_t SyncTimeStamp;
} __attribute__((packed)) ext_game_state_t;

typedef struct // 0002
{
    uint8_t winner;
} __attribute__((packed)) ext_game_result_t;

typedef struct
{
    uint16_t red_1_robot_HP;
    uint16_t red_2_robot_HP;
    uint16_t red_3_robot_HP;
    uint16_t red_4_robot_HP;
    uint16_t red_5_robot_HP;
    uint16_t red_7_robot_HP;
    uint16_t red_outpost_HP;
    uint16_t red_base_HP;
    uint16_t blue_1_robot_HP;
    uint16_t blue_2_robot_HP;
    uint16_t blue_3_robot_HP;
    uint16_t blue_4_robot_HP;
    uint16_t blue_5_robot_HP;
    uint16_t blue_7_robot_HP;
    uint16_t blue_outpost_HP;
    uint16_t blue_base_HP;
} __attribute__((packed)) ext_game_robot_HP_t;

typedef struct // 0101
{
    uint32_t event_type;
} __attribute__((packed)) ext_event_data_t;

typedef struct // 0x0102
{
    uint8_t supply_projectile_id;
    uint8_t supply_robot_id;
    uint8_t supply_projectile_step;
    uint8_t supply_projectile_num;
} __attribute__((packed)) ext_supply_projectile_action_t;

// typedef struct // 0x0103
// {
//     uint8_t supply_projectile_id;
//     uint8_t supply_robot_id;
//     uint8_t supply_num;
// } __attribute__((packed)) ext_supply_projectile_booking_t;

typedef struct // 0x0105
{
    uint8_t dart_remaining_time;
} __attribute__((packed)) ext_dart_remaining_time_t;
// typedef struct
// {
//     uint8_t level;
//     uint8_t foul_robot_id;
// } __attribute__((packed)) ext_referee_warning_t;

typedef struct // 0x0201
{
    uint8_t robot_id;
    uint8_t robot_level;
    uint16_t remain_HP;
    uint16_t max_HP;
    uint16_t shooter_id1_17mm_cooling_rate;
    uint16_t shooter_id1_17mm_cooling_limit;
    uint16_t shooter_id1_17mm_speed_limit;
    uint16_t shooter_id2_17mm_cooling_rate;
    uint16_t shooter_id2_17mm_cooling_limit;
    uint16_t shooter_id2_17mm_speed_limit;
    uint16_t shooter_id1_42mm_cooling_rate;
    uint16_t shooter_id1_42mm_cooling_limit;
    uint16_t shooter_id1_42mm_speed_limit;
    uint16_t chassis_power_limit;
    uint8_t mains_power_gimbal_output : 1;
    uint8_t mains_power_chassis_output : 1;
    uint8_t mains_power_shooter_output : 1;
} __attribute__((packed)) ext_game_robot_state_t;

typedef struct // 0x0202
{
    uint16_t chassis_volt;
    uint16_t chassis_current;
    float chassis_power;
    uint16_t chassis_power_buffer;
    uint16_t shooter_id1_17mm_cooling_heat;
    uint16_t shooter_id2_17mm_cooling_heat;
    uint16_t shooter_id1_42mm_cooling_heat;
} __attribute__((packed)) ext_power_heat_data_t;

typedef struct // 0x0203
{
    float x;
    float y;
    float z;
    float yaw;
} __attribute__((packed)) ext_game_robot_pos_t;

typedef struct // 0x0204
{
    uint8_t power_rune_buff;
} __attribute__((packed)) ext_buff_musk_t;

typedef struct // 0x0205
{
    uint8_t attack_time;
} __attribute__((packed)) aerial_robot_energy_t;

typedef struct // 0x0206
{
    uint8_t armor_type : 4;
    uint8_t hurt_type : 4;
} __attribute__((packed)) ext_robot_hurt_t;

typedef struct // 0x0207
{
    uint8_t bullet_type;
    uint8_t shooter_id;
    uint8_t bullet_freq;
    float bullet_speed;
} __attribute__((packed)) ext_shoot_data_t;
typedef struct // 0x0208
{
    uint16_t bullet_remaining_num_17mm;
    uint16_t bullet_remaining_num_42mm;
    uint16_t coin_remaining_num;
} __attribute__((packed)) ext_bullet_remaining_t;

typedef struct // 0x0209
{
    uint32_t rfid_status;
} __attribute__((packed)) ext_rfid_status_t;
typedef struct // 0x0301
{
    uint16_t send_ID;
    uint16_t receiver_ID;
    uint16_t data_cmd_id;
    uint16_t data_len;
    uint8_t *data;
} __attribute__((packed)) ext_student_interactive_data_t;

typedef struct
{
    float data1;
    float data2;
    float data3;
    uint8_t data4;
} __attribute__((packed)) custom_data_t;

typedef struct
{
    uint8_t data[64];
} __attribute__((packed)) ext_up_stream_data_t;

typedef struct
{
    uint8_t data[32];
} __attribute__((packed)) ext_download_stream_data_t;

extern void init_referee_struct_data(void);
extern void referee_data_solve(uint8_t *frame);

extern void get_chassis_power_and_buffer(fp32 *power, fp32 *buffer, fp32 *powerlimit);

extern uint8_t get_robot_id(void);

extern void get_shoot_heat0_limit_and_heat0(uint16_t *heat0_limit, uint16_t *heat0);
extern void get_shoot_heat1_limit_and_heat1(uint16_t *heat1_limit, uint16_t *heat1);
#endif
