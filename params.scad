
// Params for GT3 TimingBelt and accesories:
// belt
belt_width = 5;

// pulley
pulley_belt_width = belt_width * 1.2;


// General Params for the sketch
height = 800;
width = 600;
depth = 300;

profile_w = 20;

u_lower_rad = 70;
u_upper_rad = 20 + u_lower_rad;
u_lower_h = 20;
u_upper_h = 20;
u_main_hole_rad = 50;
rings_z_dist = 5;


// Params for main bearing plate
//   with its two rings:
glassBall_r = 8;
mainHole_r = 20;
wall_w = 4;

ring_base_w = 2*glassBall_r + 2*wall_w;

// the angle (from horisontal) of 
//   the force ransform of the glassBalls: 
ring_contact_angle = 30;

// diff between the radiuses
//   of uper and lower rings:
ring_diff_r = 2*glassBall_r * sin(ring_contact_angle);

// the height of the slot between the rings:
ring_diff_h = glassBall_r/2;

bigger_ring_r = mainHole_r + ring_diff_r;
lower_ring_h = ring_base_w;

// maximum radious of all rings in this model:
max_r = mainHole_r + ring_base_w + 30;



