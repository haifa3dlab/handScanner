include <params.scad>
include <util.scad>
use <TimingBelt.scad>

// params:
rings_bearing_angle = 45;

bearing_holder_clip_w = 0.2; //1.5;
bearing_touch_tol = 0.2; // tolerance to objects which touch and move with the bearing surface.
bearing_rotate_tol = 0.8; //  tolerance to objects which the bearing rotates relative to them
bearing_rod_l = 3*bearing_h + bearing_holder_clip_w;
bearing_rod_bottom_l = bearing_rod_l - bearing_h/2 - bearing_holder_clip_w;
bearing_rod_angle = 90 - rings_bearing_angle; // to horisontal rings

// width of the shelf of the bearing_rod
//   which stops the bearing to slide down it.
bearing_rod_stop_shelf_w = 1.5;
bearing_rod_max_r = bearing_in_d/2 + bearing_rod_stop_shelf_w;
bearing_rod_push_tol = 0.05;

bearing_rod_top_hole_d = M3_d;
bearing_rod_top_hole_h = 6;


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
upper_ring_bearing_touch_r = bearings_center_r
  - bearing_out_d/2 * cos(rings_bearing_angle)
  - bearing_h/2 * sin(90 - rings_bearing_angle);

wall_bearing_center_z_dist =
  wall_w * tan(rings_bearing_angle) +
  bearing_h/2 / sin(90 - rings_bearing_angle) +
  bearing_rotate_tol;

// the height difference between where
//   the bearing touches upper ring and
upper_ring_touch_bearing_center_diff_z =
  bearing_out_d/2 * sin(rings_bearing_angle) -
  bearing_h/2 * sin(90 - rings_bearing_angle);

// tolerance of the height at which
//   upper_ring touches bearing:
bearing_touch_z_tol = 1;

// the part of upper ring which is
//   vertical and goes under the bearing touch point
upper_ring_under_bearing_z =
  upper_ring_touch_bearing_center_diff_z -
  2*bearing_rotate_tol;


echo("wall_bearing_center_z_dist = ", wall_bearing_center_z_dist);

module ring_cut_conus(bot_r) {
	conus_h = bot_r * tan(rings_bearing_angle);
	cylinder(h = conus_h, r1 = 0, r2 = bot_r);
}

module bearing() 
{ // centralized:
  translate([0, 0, -bearing_h/2])
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

module first_bearing_in_place()
{
  translate([bearings_center_r, 0, bearings_center_z])
    rotate([0, rings_bearing_angle, 0])
      bearing();
}

module all_bearings() {
  for (i = [0:3])
  {
    angle = i*90;        
    rotate([0,0,angle])
      first_bearing_in_place();
  }
}

module first_bearing_rod()
{
  translate([rod_center_r, 0, rod_center_z])
    rotate([0, (90 - bearing_rod_angle), 0])
  difference() {
    union() {
      cylinder(r = bearing_in_d/2 - bearing_rod_push_tol, h = bearing_rod_l, center = true, $fn = 50);

      // clips on the tip of the rod to hold bearing is place:
      /*
      translate([0, 0, bearing_rod_l/2 - bearing_holder_clip_w])
        cylinder(r1 = bearing_in_d/2,
                 r2 = bearing_in_d/2 + bearing_holder_clip_w,
                 h = bearing_holder_clip_w, $fn = 50);
      */

      translate([0, 0, -bearing_rod_l/2])
        cylinder(r = bearing_rod_max_r,
                 h = bearing_rod_l - bearing_holder_clip_w - bearing_h,
                 $fn = 50);
    }
    
    // cylinder to receive M3 
    translate([0, 0, bearing_rod_l/2 - bearing_rod_top_hole_h/2 + epsilon])
      cylinder(r = bearing_rod_top_hole_d/2,
               h = bearing_rod_top_hole_h,
               center = true, $fn = 20);
    
    // cut slot for clips to bend:
    //union() {
    //}
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
                     M4hole_d/2); 
      
      translate([0, lower_ring_out_r - fasten_plate_w, 0])
        fasten_plate(fasten_plate_l,
                     fasten_plate_w,
                     fasten_plate_h,
                     M4hole_d/2); 
    }
    
    // central hole:
    translate([0, 0, -epsilon])
      cylinder(r = lower_ring_in_r,
               h = bearings_center_z, $fn = 100);
    
    all_rod_mount_holes();
  }
}


module belt_slide_stopper(in_r, height, is_below_belt)
{
  if (is_below_belt) {
    cylinder(r1 = in_r + 2*height, r2 = in_r,
           h = height, $fn = 100);
  } else {
    cylinder(r1 = in_r, r2 = in_r + 2*height,
           h = height, $fn = 100);
  }
}

upper_ring_slope_len =
  bearing_h + 2*bearing_touch_z_tol;

upper_ring_slope_h = upper_ring_slope_len * sin(90 - bearing_rod_angle);

horis_ring_h = wall_w/2;

upper_ring_h =
  upper_ring_under_bearing_z
  + upper_ring_slope_h
  + horis_ring_h
  + 2*wall_w
  + pulley_real_h;

upper_ring_slope_out_r =
  upper_ring_bearing_touch_r +
  upper_ring_slope_len * cos(90 - bearing_rod_angle);


module upper_ring() {
  translate([0, 0, bearings_center_z + 2*bearing_rotate_tol])
  difference() {
    union() {
      // lower vertical ring
      cylinder(r = upper_ring_bearing_touch_r,
               h = upper_ring_under_bearing_z, $fn = 100);

      // slanted ring
      translate([0, 0, upper_ring_under_bearing_z - epsilon])
        cylinder(r1 = upper_ring_bearing_touch_r,
                 r2 = upper_ring_slope_out_r,
                 h =  upper_ring_slope_h + 2*epsilon,
                 $fn = 100);

      // horisontal bearing stopper from above:
      horis_ring_z = upper_ring_under_bearing_z + upper_ring_slope_h;
      translate([0, 0, horis_ring_z - epsilon])
        cylinder(r = upper_ring_slope_out_r + 2*wall_w,
                 h = horis_ring_h + 2*epsilon, $fn = 100);

      // belt slide stopper from above:
      translate([0, 0, horis_ring_z + horis_ring_h - epsilon])
        belt_slide_stopper(upper_ring_slope_out_r, 
                           wall_w + 2*epsilon, true);
        
      // pulley teeth ring:
      translate([0, 0, horis_ring_z + horis_ring_h + wall_w - epsilon])
        scale([1, 1, 1.2])
          import("pulley_r35.3.stl", convexity=4);
          // create using drawPulley(2*upper_ring_slope_out_r, false) below
          // cylinder(r=upper_ring_slope_out_r, h=7.8);

      // belt slide stopper from above:
      translate([0, 0, horis_ring_z + horis_ring_h + wall_w + pulley_real_h - 1])
        belt_slide_stopper(upper_ring_slope_out_r, wall_w, false);
    }

    // central hole:
    translate([0, 0, upper_ring_h/2 - epsilon]) 
      union() {
        cube([profile2020_w, profile2020_w,
              upper_ring_h + 1], center = true);

        rotate([0, 0, 45])
        translate([(mainHole_r + profile2020_w)/2, 0, 0]) // middle of wall
          cube([usb_hole_w, usb_hole_l,
                upper_ring_h + 1], center = true);
      }

    /*
    cylinder(r = mainHole_r,
            h = upper_ring_h + 2*epsilon, $fn = 100);
    */
  } // difference
}


// tests:

/*
// test bearings_center rad and height:
color("blue") 
  ring(lower_ring_in_r, 
           bearings_center_r,
           bearings_center_z,
           fn = 100);
*/


color("black")
  rotate([0, 45, 0])
    translate([0, 0, 20])
      first_bearing_rod();



// results:

//color("green") lower_ring();

//color("black") all_bearing_rods();

//color("blue") all_bearings();

// create pulley for this upper plate:
//echo("making pulley with radious = ", upper_ring_slope_out_r);
//color("yellow") drawPulley(2*upper_ring_slope_out_r, false);

//color("red") upper_ring();

