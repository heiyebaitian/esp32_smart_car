#include "joystick_analysis.h"
#include <stdio.h>
#include <math.h>

const double joystick_adsorption_range = 3; // 摇杆吸附范围（度数 0~360） 注意：实际吸附扇形范围为值的2倍
const double joystick_inside_DZ = 0.05; // 摇杆内部死区范围（百分比 0~1.00）
const double joystick_outside_DZ = 0.05; // 摇杆外部死区范围（百分比 0~1.00）

/* 摇杆角度计算 */
double calculate_angle(double x, double y, int max_coord) {
  if(fabs(x) > (max_coord * 2 + 1) || fabs(y) > (max_coord * 2 + 1)) return 0; // 输入值超限

  // 计算弧度
  x = max_coord - x;
  y = y - max_coord;
  double angle_in_radians = atan2(y, x);
  // 转换为度数
  double angle_in_degrees = angle_in_radians * (180.0 / M_PI) + 180;

  return angle_in_degrees;
}

/* 摇杆角度吸附处理 */
double handle_joystick_adsorption(double angle_in_degrees){
    if(angle_in_degrees > 360 || angle_in_degrees < 0) return 0; // 输入值不合法

    // 摇杆吸附处理
    if(angle_in_degrees <= joystick_adsorption_range || angle_in_degrees >= 360 - joystick_adsorption_range){return 0;} // 右吸附
    else if(angle_in_degrees >= 90 - joystick_adsorption_range && angle_in_degrees <= 90 + joystick_adsorption_range){return 90;} //上吸附
    else if(angle_in_degrees >= 180 - joystick_adsorption_range && angle_in_degrees <= 180 + joystick_adsorption_range){return 180;} //左吸附
    else if(angle_in_degrees >= 270 - joystick_adsorption_range && angle_in_degrees <= 270 + joystick_adsorption_range){return 270;} //下吸附
    return angle_in_degrees;
}

/* 死区范围处理 */
double handle_joystick_dead_zone(double percentage){
  if(percentage < 0 || percentage > 1) return 0; // 输入值不合法

  if(percentage <= joystick_inside_DZ) return 0; // 内部死区设定
  if(percentage >= 1 - joystick_outside_DZ) return 1; // 外部死区设定
  return percentage;
}

/* 摇杆向量长度计算 */
double calculate_length(double x, double y, int max_coord) {
    // 计算向量的长度
    x = x - max_coord;
    y = y - max_coord;
    return sqrt(x * x + y * y);
}

/* 摇杆方向最大向量长度计算 */
double calculate_max_length(double angle_degrees, int max_coord) {
    // 将角度转换为弧度
    double angle_radians = angle_degrees * (M_PI / 180.0);

    // 计算在给定角度下的x和y分量的最大可能值
    double x_component = fabs(cos(angle_radians));
    double y_component = fabs(sin(angle_radians));

    // 确保x和y分量不会超出范围，同时最大化它们的值
    double max_factor = fmin(1.0 / x_component, 1.0 / y_component);
    
    x_component *= max_coord * max_factor;
    y_component *= max_coord * max_factor;

    // 使用勾股定理计算最大长度
    double max_length = sqrt(x_component * x_component + y_component * y_component);

    return max_length;
}

/* 摇杆杆量百分比计算 */
double calculate_joystick_percentage(double x, double y, int max_coord) {
  double XY_angle = calculate_angle(x, y, max_coord);
  double length = calculate_length(x, y, max_coord);
  double max_lenght = calculate_max_length(XY_angle, max_coord);
  double percentage = length / max_lenght;

  if(percentage >= 1) {return 1;}
  if(percentage <= 0) {return 0;}

  return percentage;
}

/* 麦克纳姆轮A组分量计算 */
double groupA_motor_percentage(double XY_angle,double percentage) {
    double MK_angle = XY_angle - 45;
    double MK_length = calculate_max_length(MK_angle , 100) * percentage;
    double MK_groupA_percentage = (MK_length * cos(MK_angle * (M_PI / 180.0))) / 100;

    if(MK_groupA_percentage >= 1) {return 1;}
    if(MK_groupA_percentage <= -1) {return -1;}

    return MK_groupA_percentage;
}

/* 麦克纳姆轮B组分量计算 */
double groupB_motor_percentage(double XY_angle,double percentage) {
    double MK_angle = XY_angle - 45;
    double MK_length = calculate_max_length(MK_angle , 100) * percentage;
    double MK_groupB_percentage = (MK_length * sin(MK_angle * (M_PI / 180.0))) / 100;

    if(MK_groupB_percentage >= 1) {return 1;}
    if(MK_groupB_percentage <= -1) {return -1;}
    
    return MK_groupB_percentage;
}