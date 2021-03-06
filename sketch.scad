include <params.scad>
include <util.scad>
use <torusPart.scad>

module shell_skeleton() {
  difference() {
    cube([width, depth, height]);
    
    // remove front  
    translate([profile_w,-profile_w,-epsilon])
      cube([width-2*profile_w, depth, height-profile_w+epsilon]);
    
    // remove back  
    translate([profile_w,profile_w,profile_w])
      cube([width-2*profile_w, depth, height-2*profile_w]);
    
    // remove sides 
    translate([-profile_w,profile_w,profile_w])
      cube([width+2*profile_w, depth-2*profile_w, height-2*profile_w]);
    
    // remove top 
    translate([profile_w,profile_w,0])
      cube([width-2*profile_w, depth-2*profile_w, height+2*profile_w]);
  }
}


module lower_ring() {
  translate([width/2, depth/2, height])
    ring(u_main_hole_rad, u_lower_rad, u_lower_h);
}

module lower_ring_mount() {
  translate([width/2, 0, height - profile_w]) {
    union() {
      translate([-u_lower_rad, 0, 0])
        cube([profile_w, depth, profile_w]);
  
      translate([u_lower_rad - profile_w, 0, 0])
        cube([profile_w, depth, profile_w]);
    }
  }
}

upper_ring_elev = height + u_lower_h + rings_z_dist;

module upper_ring() {
  translate([width/2, depth/2, upper_ring_elev])
    ring(u_main_hole_rad, u_upper_rad, u_upper_h);
}


module stepper() {
  union() {
    cube([stepper_w, stepper_w, stepper_box_h]);

    translate([stepper_w/2, stepper_w/2, stepper_box_h - epsilon])
      cylinder(r = stepper_axis_rad,
        h = stepper_axis_len + epsilon, $fn = 50);
  }
}


upper_stepper_x = width/2 - u_upper_rad - 1.5*stepper_w;

// upper stepper:
color("red") {
  translate([upper_stepper_x,
             depth/2,
             upper_ring_elev + u_upper_h - stepper_all_h])
    stepper();
}

// upper stepper mount:
translate([upper_stepper_x - profile_w, 0, height - profile_w])
      cube([profile_w, depth, profile_w]);

module axis() {
  axis_h = height - axis_bot_elev + axis_top_above_upper_plate;
  translate([width/2 - profile_w/2, depth/2 - profile_w/2, axis_bot_elev])
    cube([profile_w, profile_w, axis_h]);
}


slider_w = slider_column_w + 2*slider_pole_w;
slider_d = 3*slider_pole_w;

module slider(slider_h) {
  hole_r = slider_pole_w + slider_tol;
  difference() {
    translate([-slider_d/2, -slider_w/2, 0])
      cube([slider_d, slider_w, slider_h]);

    translate([0, -slider_column_w/2 + slider_pole_w, -epsilon])
        cylinder(r = hole_r, h = slider_h+2*epsilon, $fn = 50);

    translate([0, +slider_column_w/2 - slider_pole_w, -epsilon])
        cylinder(r = hole_r, h = slider_h+2*epsilon, $fn = 50);
  }
}


slider_column_h = axis_bot_elev + profile_w - min_camera_z;

module slideColumn(slider_h) {
  union() {
    translate([0, 0, slider_column_h - profile_w/2])
      Y_cylinder(profile_w/2, slider_column_w);

    translate([0, -slider_column_w/2 + slider_pole_w, 0])
        cylinder(r = slider_pole_w, h = slider_column_h, $fn = 50);

    translate([0, +slider_column_w/2 - slider_pole_w, 0])
        cylinder(r = slider_pole_w, h = slider_column_h, $fn = 50);

    translate([0, 0, slider_column_h/2 - slider_h/2])
      slider(slider_h);
  }
}

module third_pole_and_mount() {
  union() {
    holder_l = slider_column_w/2 * sin(60);
    translate([-holder_l/2, 0/*-profile_w*/, slider_column_h - profile_w/2])
      X_cylinder(profile_w/2, holder_l);
    
    translate([-holder_l, 0, 0])
      cylinder(r = slider_pole_w, h = slider_column_h, $fn = 50);
    
    // counter-balances:
    translate([-holder_l, 0, slider_column_h/2 - camera_h])
      cylinder(r = 2*slider_pole_w, h = 2*camera_h, $fn = 50);
  }
}

module camera() {
  union() {
    cube([camera_w, camera_d, camera_h]);

    translate([camera_h/8, 0, camera_h/2])
      Y_cylinder(camera_h/16, 2*relief_depth);
    translate([5*camera_h/8, 0, camera_h/2])
      Y_cylinder(camera_h/32, 2*relief_depth);
    translate([camera_w/4, 0, camera_h/2])
      Y_cylinder(camera_h/8, 2*relief_depth);
    translate([camera_w - 1.3*camera_h, 0, camera_h/2])
      Y_cylinder(camera_h/16, 2*relief_depth);

    translate([camera_w - camera_h,
               -relief_depth, +relief_depth])
      cube([camera_h - 2*relief_depth,
            2*relief_depth,
            camera_h - 2*relief_depth]);

    translate([camera_w - camera_h/2, 0, camera_h/2])
      Y_cylinder(camera_h/6, 4*relief_depth);
  }
}


module hand_holder() {
  translate([width/2, depth/2, hand_support_cutR])
    torus(hand_support_bigR, hand_support_cutR);

  translate([width/2, depth/2, hand_lock_elev + hand_lock_cutR])
    torus(hand_lock_bigR, hand_lock_cutR);

  translate([width/2, depth/2 + hand_lock_bigR, 0])
    cylinder(r = hand_lock_cutR,
             h = hand_lock_elev + hand_lock_cutR,
             $fn = 50);

  translate([width/2 - profile_w/2,
             depth/2 + hand_lock_bigR, 0])
    cube([profile_w, depth/2 - hand_lock_bigR, profile_w]);
}


module show_rotating_mount(angle = 40, num_columns = 1)
{
  translate([width/2, depth/2 + axis_y_offset, 0])
  rotate([0, 0, angle])
  translate([-width/2, -depth/2, 0])
  {
    color("black") axis();

    // main horizontal beam:
    color("black")
      translate([width/2 - camera_mount_rad, depth/2 - profile_w/2, axis_bot_elev])
        cube([2*camera_mount_rad, profile_w,  profile_w]);

    // lower stepper mount:
    if (num_columns > 1) {
      color("black")
        translate([width/2 - stepper_w - profile_w,
                   depth/2  - profile_w/2,
                   axis_bot_elev])
          cube([stepper_w, stepper_all_h - profile_w/2,  profile_w]);
    }
    
    // lower stepper
    color("red") {
//
        stepper_x = ((num_columns < 2) ? width/2 - stepper_w - camera_mount_rad : width/2 - stepper_w - profile_w);
//      } else {
//        stepper_x = 
//      }
      translate([stepper_x,
                 depth/2 + stepper_all_h - 1.5*profile_w,
                 axis_bot_elev + profile_w])
        rotate([90, 0, 0])
          stepper();
    }
    
    // camera column:
    color("black") {
      translate([width/2 - camera_mount_rad, depth/2, min_camera_z]) {
        slideColumn(camera_h);
      
        if (num_columns < 2) {
          third_pole_and_mount();        
        }
      }
    }

    // balance column or only static weight:
    color("black") {
      if (num_columns > 1) {
          translate([width/2 + camera_mount_rad, depth/2, min_camera_z])
            slideColumn(3*camera_h);
      } else {
        cyl_r = 1.5*profile_w;
        cyl_l = 3*camera_h;
        translate([width/2 + camera_mount_rad - cyl_l/2,
                   depth/2, axis_bot_elev + profile_w/2])
          X_cylinder(cyl_r, cyl_l);
      }
    }
    
    // camera
    color("red")
      translate([width/2 - camera_mount_rad + slider_d,
                 depth/2 - camera_w/2,
                 min_camera_z + slider_column_h/2 - camera_h/2])
        rotate([0, 0, 90])
          camera();
  }
}

module hex_cage() {

   hex_on_layer();

  translate([0,0, slider_column_h])
      hex_on_layer();
}

module hex_on_layer() {
    hex_r = camera_mount_rad;
    difference() {
    cylinder(r=hex_r, h = 40, $fn=6, center = true);
        cylinder(r=hex_r-profile_w , h = 41, $fn=6, center = true); 
    } 
}

/*
color("magenta")
translate([width/2, depth/2 + axis_y_offset, min_camera_z]) 
    rotate([0,0, 0 ]) 
 hex_cage();
*/

// results:

shell_skeleton();

color("green") hand_holder();

lower_ring_mount();

color("green") lower_ring();

color("red") upper_ring();

// First version:
//show_rotating_mount(40, num_columns = 2);

// ver2:
show_rotating_mount(40, num_columns = 1);


