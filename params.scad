
// Params for GT3 TimingBelt and accesories:
// belt
belt_width = 6;

// pulley
pulley_belt_width = belt_width * 1.2;


// General Params for the sketch
height = 700;
width = 600;
depth = 400;

profile_w = 20;

u_lower_rad = 70;
u_upper_rad = 20 + u_lower_rad;
u_lower_h = 20;
u_upper_h = 20;
u_main_hole_rad = 50;
rings_z_dist = 5;
axis_y_offset = 0;

stepper_w = 42.3;
stepper_box_h = 40;
stepper_axis_rad = 2.5;
stepper_axis_len = 24+2; // above box
stepper_all_h = stepper_box_h + stepper_axis_len;

axis_bot_elev = height - 90;
axis_top_above_upper_plate = 80;

camera_mount_rad = 275;
min_camera_z = 300;

slider_pole_w = 8;
slider_column_w = 80;

// tolerance for holes sliding on column:
slider_tol = 0.5;

camera_w = 140;
camera_d = 15;
camera_h = 40;

relief_depth = 2;

hand_support_bigR = 50;
hand_support_cutR = 20;
hand_lock_bigR = 65;
hand_lock_cutR = 10;
hand_lock_elev = 140;


// Params for main rotation plate
//   with its two rings:
glassBall_r = 8;
mainHole_r = 20;
wall_w = 4;

ring_base_w = 2*glassBall_r + 2*wall_w;
upper_ring_out_r = mainHole_r + ring_base_w;

// the angle (from horisontal) of 
//   the force ransform of the glassBalls: 
ring_contact_angle = 30;

// diff between the radiuses
//   of uper and lower rings:
ring_diff_r = 2*glassBall_r * sin(ring_contact_angle); // ?? - can be 0 

// the height of the slot between the rings:
ring_diff_h = glassBall_r/2;

bigger_hole_r = mainHole_r + ring_diff_r;
lower_ring_h = ring_base_w;
lower_ring_max_r = bigger_hole_r + ring_base_w;

fasten_plate_h = 1.5*wall_w;

// maximum radious of all rings in this model:
max_r = mainHole_r + ring_base_w + 30;

pulley_real_h = 9.2;

usb_hole_l = 16;
usb_hole_w = 8;


// when using bearings:
bearing_in_d = 8;
bearing_out_d = 22;
bearing_h = 7;

big_washer_d = 19;

bearing_axis_d = bearing_in_d - 0.1;
bearing_axis_l = 60;


// Params for mounted camera slider:

// tolerance for a hole which should turn around
//   some plastic pole
pole_turn_tol = 0.15; // on each side

profile2020_w = 20;
profile2020_tol = 0; // tolerance on each side (0 is OK)

M3_d = 2;
M3hole_d = M3_d + 0.3;
M4_d = 4;
M4hole_d = M4_d + 3;
hole_wall_dist = 4;

prof2020_fasten_plate_l = hole_wall_dist + wall_w + M4hole_d;

no_bend_beam_w = 2*wall_w;

fasten_plate_hole_dist = 10;
fasten_plate_l = 100;
fasten_plate_w = 120;
fasten_plate_h = wall_w;

rod_d = 8;
rod_holder_tol = 0; // on each side
rod_holder_fasten_block_l = 2*hole_wall_dist +
  2*fasten_plate_hole_dist + 2*M3hole_d/2;
rod_holder_fasten_block_w = 3*wall_w;
rod_holder_h = 4*wall_w;
rod_holder_fasten_h = 4*wall_w; // only the back fastener

camera_rod_centers_dist = 45;

holder_bend_len = 4;
holder_bend_close_gap = 3 + 2*rod_holder_tol;

stepper_holder_l = 4*wall_w;

linear_bearing_in_d = 8;
linear_bearing_out_d = 15;
linear_bearing_h = 24;
linear_bearing_holder_wall_w = 3;
linear_bearing_holder_gap_w = 1;
linear_bearing_holder_tol = 0; //-0.05;

linear_bearing_holder_out_r =
    linear_bearing_out_d/2
    + linear_bearing_holder_tol
    + linear_bearing_holder_wall_w;

