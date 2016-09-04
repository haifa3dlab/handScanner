height = 800;
width = 600;
depth = 300;

profile_w = 20;
epsilon = 5;

u_inner_rad = 70;
u_outter_rad = 20 + inner_rad;
u_inner_h = 20;
u_outter_h = 20;
//rings_z_diff = 15;


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

color("black") 
   differ
{
  translate([width/2, depth/2, height+inner_h])
  cylinder(r=outter_rad, h=outter_h);
    
   
}

color("green")
  translate([width/2, depth/2, height])
    ring



module ring(in_rad, out_rad, height) {
    difference() {
        cylinder(r = out_rad, h = height);
        translate([0,0,epsilon])
           cylinder(r = in_rad, h = height+2*epsilon);
    }
    
}
