#include "joystick_analysis.h"
#include <stdio.h>
#include <math.h>



double calculate_angle(double x, double y, int max_coord) {
    // 计算弧度
    x = max_coord - x;
    y = y - max_coord;
    double angle_in_radians = atan2(y, x);
    // 转换为度数
    double angle_in_degrees = angle_in_radians * (180.0 / M_PI) + 180;
    return angle_in_degrees;
}

double calculate_length(double x, double y, int max_coord) {
    // 计算向量的长度
    x = x - max_coord;
    y = y - max_coord;
    return sqrt(x * x + y * y);
}

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

double calculate_joystick_percentage(double x, double y, int max_coord) {
  double XY_angle = calculate_angle(x, y, max_coord);
  double length = calculate_length(x, y, max_coord);
  double max_lenght = calculate_max_length(XY_angle, max_coord);
  double percentage = length / max_lenght;

  if(percentage >= 100) {return 100;}
  if(percentage <= 0) {return 0;}

  return percentage;
}

double groupA_motor_percentage(double XY_angle,double percentage) {
    double MK_angle = XY_angle - 45;
    double MK_length = calculate_max_length(MK_angle , 100) * percentage;
    double MK_groupA_percentage = (MK_length * cos(MK_angle * (M_PI / 180.0))) / 100;

    if(MK_groupA_percentage >= 1) {return 1;}
    if(MK_groupA_percentage <= -1) {return -1;}

    return MK_groupA_percentage;
}

double groupB_motor_percentage(double XY_angle,double percentage) {
    double MK_angle = XY_angle - 45;
    double MK_length = calculate_max_length(MK_angle , 100) * percentage;
    double MK_groupB_percentage = (MK_length * sin(MK_angle * (M_PI / 180.0))) / 100;

    if(MK_groupB_percentage >= 1) {return 1;}
    if(MK_groupB_percentage <= -1) {return -1;}
    
    return MK_groupB_percentage;
}