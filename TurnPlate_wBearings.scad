include <params.scad>
include <util.scad>

// params:
rings_bearing_angle = 45;

bearing_holder_clip_w = 0.5;
bearing_touch_tol = 0.2; // tolerance to objects which touch and move with the bearing surface.
bearing_rotate_tol = 2; //  tolerance to objects which the bearing rotates relative to them
bearing_rod_l = 3*bearing_h + bearing_holder_clip_w;
bearing_rod_bottom_l = bearing_rod_l - bearing_h/2 + bearing_holder_clip_w;
bearing_rod_angle = 90 - rings_bearing_angle; // to horisontal rings
// width of the shelf of the bearing_rod
//   which stops the bearing to slide down it.
bearing_rod_stop_shelf_w = 1.5;
bearing_rod_max_r = bearing_in_d/2 + bearing_rod_stop_shelf_w;
bearing_rod_push_tol = 0.1;

// Distance of the center of each bearing
//   from rings' axis: 
bearings_center_r = mainHole_r + wall_w +
  bearing_rod_bottom_l * cos(bearing_rod_angle) +
  bearing_in_d/2 * cos(90 - bearing_rod_angle);

bearings_center_z = wall_w +
  bearing_rod_max_r * sin(90 - bearing_rod_angle) +
  bearing_rod_bottom_l * sin(bearing_rod_angle);

// Radious at which the upper ring
//   touches the point at the bearing 
//   which is closest to the center axis of the rings:
upper_ring_bearing_touch_r = bearings_center_r - 
  bearing_out_d/2 * cos(rings_bearing_angle);

wall_bearing_center_z_dist = 
  wall_w * tan(rings_bearing_angle) +
  bearing_h/2 / sin(90 - rings_bearing_angle) +
  bearing_rotate_tol;

echo("wall_bearing_center_z_dist = ", wall_bearing_center_z_dist);

module ring_cut_conus(bot_r) {
	conus_h = bot_r * tan(rings_bearing_angle);
	cylinder(h = conus_h, r1 = 0, r2 = bot_r);
}

module bearing() {
  ring(bearing_in_d/2, bearing_out_d/2, bearing_h);
}


lower_ring_in_r = mainHole_r;
lower_ring_out_r = bearings_center_r + wall_w;

rod_center_r = mainHole_r + wall_w +
  bearing_rod_l/2 * cos(bearing_rod_angle) +
  bearing_in_d/2 * cos(90 - bearing_rod_angle);

rod_center_z = wall_w +
  bearing_rod_l/2 * sin(bearing_rod_angle) +
  bearing_rod_max_r * sin(90 - bearing_rod_angle);


module first_bearing_rod()
{
  translate([rod_center_r, 0, rod_center_z])
    rotate([0, (90 - bearing_rod_angle), 0])
  difference() {
    union() {
      cylinder(r = bearing_in_d/2, h = bearing_rod_l, center = true, $fn = 50);

      translate([0, 0, bearing_rod_l/2 - bearing_holder_clip_w])
        cylinder(r1 = bearing_in_d/2,
                 r2 = bearing_in_d/2 + bearing_holder_clip_w,
                 h = bearing_holder_clip_w, $fn = 50);

      translate([0, 0, -bearing_rod_l/2])
        cylinder(r = bearing_rod_max_r,
                 h = bearing_rod_l - bearing_holder_clip_w - bearing_h,
                 $fn = 50);
    }

    // cut slot for clips to bend:
    union() {
    }
  }
}

module all_bearing_rods() {
  for (i = [0:3])
  {
    angle = i*90;        
    rotate([0,0,angle])
      first_bearing_rod();
  }
}

module first_rod_mount_hole()
{
  translate([rod_center_r, 0, rod_center_z])
    rotate([0, (90 - bearing_rod_angle), 0])
      cylinder(r = bearing_rod_max_r + bearing_rod_push_tol, h = bearing_rod_l, center = true, $fn = 50);
}

module all_rod_mount_holes() {
  for (i = [0:3])
  {
    angle = i*90;        
    rotate([0,0,angle])
      first_rod_mount_hole();
  }
}


module fasten_plate(x, y, z, hole_r) {
  // symmetric around y
  translate([-x/2, 0, 0])
    difference() {
      cube([x, y, z]);
      
      // put hole in the middle of each side square:
      translate([y/2, y/2, -epsilon])
        cylinder(r = hole_r, h = z+2*epsilon, $fn = 50);
      
      // put hole in the middle of each side square:
      translate([x - y/2, y/2, -epsilon])
        cylinder(r = hole_r, h = z+2*epsilon, $fn = 50);
    }
}

module lower_ring() {
  rod_holder_top_r =  lower_ring_out_r -
    wall_bearing_center_z_dist / tan(rings_bearing_angle);
  rod_holder_h =
    wall_bearing_center_z_dist * tan(rings_bearing_angle);
  
  fasten_plate_w = big_washer_d + 2;
  fasten_plate_l = 2*rod_holder_top_r + 2*fasten_plate_w;
  fasten_plate_h = 1.5*wall_w;

  difference() {
    union() {
      ring(lower_ring_in_r, 
           lower_ring_out_r,
           bearings_center_z - wall_bearing_center_z_dist, fn = 100);
      
      translate([0, 0, 
        bearings_center_z - wall_bearing_center_z_dist - epsilon])
        cylinder(r1 = lower_ring_out_r,
                 r2 = rod_holder_top_r,
                 h = rod_holder_h, $fn = 100);
      
      //plates with M4 holes to fasten to prof2020
      translate([0, -lower_ring_out_r, 0])
        fasten_plate(fasten_plate_l,
                     fasten_plate_w,
                     fasten_plate_h,
                     M4hole_d); 
      
      translate([0, lower_ring_out_r - fasten_plate_w, 0])
        fasten_plate(fasten_plate_l,
                     fasten_plate_w,
                     fasten_plate_h,
                     M4hole_d); 
    }
    
    // central hole:
    translate([0, 0, -epsilon]);
      cylinder(r = lower_ring_in_r,
               h = bearings_center_z, $fn = 100);
    
    all_rod_mount_holes();
  }
}

color("green") lower_ring();

//color("black") all_bearing_rods();

/*
// test bearings_center rad and height:
color("blue") 
  ring(lower_ring_in_r, 
           bearings_center_r,
           bearings_center_z,
           fn = 100);
*/

