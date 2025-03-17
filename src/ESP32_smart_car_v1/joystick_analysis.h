#ifndef __JOYSTICK_ANALYSIS__H
#define __JOYSTICK_ANALYSIS__H
#pragma once

double calculate_angle(double x, double y, int max_coord);
double handle_joystick_adsorption(double angle_in_degrees);
double handle_joystick_dead_zone(double percentage);
double calculate_length(double x, double y, int max_coord);
double calculate_max_length(double angle_degrees, int max_coord);
double calculate_joystick_percentage(double x, double y, int max_coord);
double groupA_motor_percentage(double XY_angle,double percentage);
double groupB_motor_percentage(double XY_angle,double percentage);

#endif