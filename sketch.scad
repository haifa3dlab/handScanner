include <params.scad>
include <util.scad>


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
