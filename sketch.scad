include <params.scad>
include <util.scad>

// shell skeleton:
difference(){
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


// lower ring:
color("green")
  translate([width/2, depth/2, height])
    ring(u_main_hole_rad, u_lower_rad, u_lower_h);

// upper ring:
color("red")
  translate([width/2, depth/2, height + u_lower_h + rings_z_dist])
    ring(u_main_hole_rad, u_upper_rad, u_upper_h);

module axis() {
  axis_h = height - axis_bot_elev + axis_top_above_upper_plate;
  translate([width/2 - profile_w/2, depth/2 - profile_w/2, axis_bot_elev])
    cube([profile_w, profile_w, axis_h]);
}

color("black") axis();

color("black")
  translate([width/2 - camera_mount_rad, depth/2 - profile_w/2, axis_bot_elev])
    cube([2*camera_mount_rad, profile_w,  profile_w]);


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

module Y_cylinder(cr, clen) {
    translate([0, clen/2, 0])
      rotate([90, 0, 0])
        cylinder(r = cr, h = clen, $fn = 50);
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

color("black")
  translate([width/2 - camera_mount_rad, depth/2, min_camera_z])
    slideColumn(camera_h);

color("black")
  translate([width/2 + camera_mount_rad, depth/2, min_camera_z])
    slideColumn(3*camera_h);

//color("red") Y_cylinder(profile_w/2, slider_column_w);

color("red")
  translate([width/2 - camera_mount_rad + slider_d,
             depth/2 - camera_w/2,
             min_camera_z + slider_column_h/2 - camera_h/2])
    rotate([0, 0, 90])
      camera();

