include <params.scad>;
include <util.scad>

// profile2020 holder:

module prof2020_holder() {
  holder_w = profile2020_w + 2*wall_w; 
  holder_l = profile2020_w + 2*(hole_wall_dist + wall_w + M4hole_d);
  union() {
    difference() {
      cube([profile2020_w, holder_w, holder_w], center = true);

      cube([profile2020_w + 2*epsilon,
            profile2020_w + 2*profile2020_tol,
            profile2020_w + 2*profile2020_tol], center = true);
    }

    translate([0, 0, profile2020_w/2 + profile2020_tol + wall_w/2])
      difference() {
        cube([holder_l, profile2020_w, wall_w - profile2020_tol], center = true);

        translate([-profile2020_w/2 - hole_wall_dist - M4hole_d/2, 0, 0])
          cylinder(r = M4hole_d/2, h = wall_w+2*epsilon, center = true, $fn = 50);

        translate([profile2020_w/2 + hole_wall_dist + M4hole_d/2, 0, 0])
          cylinder(r = M4hole_d/2, h = wall_w+2*epsilon, center = true, $fn = 50);
    }

    translate([0, -(holder_w/2 + wall_w - epsilon), 0])
      cube(2*wall_w, center = true);
    translate([0, holder_w/2 + wall_w - epsilon, 0])
      cube(2*wall_w, center = true);
  }
}


// M8 rode holder:

rod_holder_in_r = rod_d/2 + rod_holder_tol;
rod_holder_out_r = rod_d/2 + rod_holder_tol + wall_w;

module rod_holder_ring() {
  ring(rod_holder_in_r,
       rod_holder_out_r,
       rod_holder_h);
}

module rod_holder() {
  union() {
    translate([0, -rod_holder_tol, 0])
      difference() {
        rod_holder_ring();
        
        translate([-rod_holder_out_r - epsilon, 0, -epsilon])
          cube([2*rod_holder_out_r + 2*epsilon,
                2*rod_holder_out_r + epsilon,
                rod_holder_h + 2*epsilon]);

        translate([-epsilon,
                   -holder_bend_close_gap/2 + epsilon,
                   -epsilon])
          cube([rod_holder_out_r + 2*epsilon,
                holder_bend_close_gap/2,
                rod_holder_h + 2*epsilon]);
      }
    translate([-rod_holder_out_r - holder_bend_len - epsilon, -rod_holder_tol - wall_w, 0])
      cube([holder_bend_len + wall_w,
            wall_w, rod_holder_h]);

    translate([0, rod_holder_tol, 0])
      difference() {
        rod_holder_ring();
        
        translate([-rod_holder_out_r - epsilon,
                   -(2*rod_holder_out_r + epsilon),
                   -epsilon])
          cube([2*rod_holder_out_r + 2*epsilon,
                2*rod_holder_out_r + epsilon,
                rod_holder_h + 2*epsilon]);
        
        translate([-epsilon, -epsilon, -epsilon])
          cube([rod_holder_out_r + 2*epsilon,
                holder_bend_close_gap/2,
                rod_holder_h + 2*epsilon]);
      }
    translate([-rod_holder_out_r - holder_bend_len - epsilon, rod_holder_tol, 0])
      cube([holder_bend_len + wall_w,
            wall_w, rod_holder_h]);
    
    // back plate:
    translate([-4*wall_w - rod_holder_out_r - holder_bend_len, -1.5*wall_w, 0]) {
      difference() {  
        cube([4*wall_w, 3*wall_w, rod_holder_h]);
      
        translate([2*wall_w, 1.5*wall_w, -epsilon])
          cylinder(r = M4hole_d/2,
                   h = rod_holder_h + 2*epsilon, $fn = 50);
      }
    }
    
    // closing holes:
    difference() {
      union() {
        translate([rod_holder_out_r - wall_w,
                   holder_bend_close_gap/2, 0])
          cube([wall_w + 3*M3hole_d,
                wall_w, rod_holder_h]);
        translate([rod_holder_out_r - wall_w,
                   -holder_bend_close_gap/2 - wall_w, 0])
          cube([wall_w + 3*M3hole_d,
                wall_w, rod_holder_h]);
      }
      translate([rod_holder_out_r + M3hole_d,
                 rod_holder_out_r, rod_holder_h/2])
        rotate([90, 0, 0])
          cylinder(r = M3hole_d/2,
                   h = 2*rod_holder_out_r, $fn = 50);
    }
  }
}


// results:

//color("black") rod_holder_ring();

color("blue") prof2020_holder();

//color("green") rod_holder();

