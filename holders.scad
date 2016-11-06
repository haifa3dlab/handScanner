include <params.scad>;
include <util.scad>
use <obiscad/bcube.scad>
use <Belt_Generator.scad>
use <camera_case.scad>

// profile2020 holder:

module prof2020_holder() {
    
  holder_w = profile2020_w + 2*wall_w; 
  holder_l = profile2020_w + 2*prof2020_fasten_plate_l;
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

/* small test cubes on the sides - unneeded:
    translate([0, -(holder_w/2 + wall_w - epsilon), 0])
      cube(2*wall_w, center = true);
    translate([0, holder_w/2 + wall_w - epsilon, 0])
      cube(2*wall_w, center = true);
*/
    fasten_w = 4*fasten_plate_hole_dist + M3hole_d + 2*wall_w;
    fasten_l = fasten_plate_hole_dist + M3hole_d + 2*wall_w;
    fasten_h = 2*wall_w;
    translate([0, 0, holder_w/2 - fasten_h/2])
      difference() {
        cube([fasten_l, fasten_w, fasten_h], center = true);

        cube([fasten_l+2*epsilon, profile2020_w, fasten_h+2*epsilon], center = true);

        translate([-fasten_l/2 + wall_w + M3hole_d/2,
                   -fasten_w/2 + wall_w + M3hole_d/2, -fasten_h/2 - epsilon])
          fasten_holes_rect(2, 1, fasten_plate_hole_dist,
                            M3hole_d/2, 3*fasten_h + 2*epsilon);
        translate([-fasten_l/2 + wall_w + M3hole_d/2,
                   fasten_w/2 - (wall_w + M3hole_d/2), -fasten_h/2 - epsilon])
          fasten_holes_rect(2, 1, fasten_plate_hole_dist,
                            M3hole_d/2, 3*fasten_h + 2*epsilon);
    }
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
    
    // back fasten block:
    translate([-rod_holder_fasten_block_l - rod_holder_out_r - holder_bend_len, -1.5*wall_w, 0]) {
      difference() {  
        cube([rod_holder_fasten_block_l,
              rod_holder_fasten_block_w,
              rod_holder_fasten_h]);

        // back alongated hole for fastening in var angles:
        translate([hole_wall_dist + M3hole_d/2,
                   rod_holder_fasten_block_w/2, -epsilon])
          cylinder(r = M3hole_d/2,
                   h = rod_holder_fasten_h + 2*epsilon, $fn = 50);
        translate([hole_wall_dist + M3hole_d/2,
                   rod_holder_fasten_block_w/2 - M3hole_d/2, -epsilon])
          cube([fasten_plate_hole_dist, M3hole_d,
                   rod_holder_fasten_h + 2*epsilon]);
        translate([hole_wall_dist + M3hole_d/2 + fasten_plate_hole_dist,
                   rod_holder_fasten_block_w/2, -epsilon])
          cylinder(r = M3hole_d/2,
                   h = rod_holder_fasten_h + 2*epsilon, $fn = 50);

        translate([hole_wall_dist + M3hole_d/2 + 2*fasten_plate_hole_dist,
                   // rod_holder_fasten_block_l - (hole_wall_dist + M3hole_d/2),
                   1.5*wall_w, -epsilon])
          cylinder(r = M3hole_d/2,
                   h = rod_holder_fasten_h + 2*epsilon, $fn = 50);
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

module rod_holder2() {
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
  
    // width of nuts so we can use two nut to connect
    nut_width = 7.2;
    block_l = 1.1*wall_w + nut_width * 2+M4hole_d;
  
    // back fasten block:
    translate([-rod_holder_fasten_block_l2 - rod_holder_out_r - holder_bend_len, 
      //-1.5*wall_w-rod_holder_fasten_block_w/2, 
      -block_l/2,
      0]) {
      difference() {  
        cube([rod_holder_fasten_block_l2,
              block_l,
              rod_holder_fasten_h]);

        // back alongated hole for fastening in var angles:
          
        translate([hole_wall_dist + M3hole_d/2,
                   block_l/2-nut_width/2-M4hole_d/2, -epsilon])
          cylinder(r = M4hole_d/2,
                   h = rod_holder_fasten_h + 2*epsilon, $fn = 50);

        translate([hole_wall_dist + M3hole_d/2,
                   block_l/2+nut_width/2+M4hole_d/2,
                   -epsilon])  {
          cylinder(r = M4hole_d/2,
                   h = rod_holder_fasten_h + 2*epsilon, $fn = 50);
                                              
                   }
   
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
      
            translate([rod_holder_out_r + M3hole_d,
                 -holder_bend_close_gap/2 - wall_w/2, rod_holder_h/2])
        rotate([90, 0, 0])
          cylinder(r = M3_nutR,
                   h = 2*rod_holder_out_r, $fn = 6);
    }
  }
}


module fasten_holes_rect(nx, ny, hole_dist, hole_r, hole_h) {
  for (ix = [0:nx - 1])
    for (iy = [0:ny - 1]) {
      translate([ix*hole_dist,
                 iy*hole_dist, 0])
        cylinder(r = hole_r, h = hole_h, $fn = 50);
  }
}

module fasten_holes_interleave(nx, ny, hole_dist, hole_r, hole_h) {
  fasten_holes_rect(nx, ny, hole_dist, hole_r, hole_h);
  translate([hole_dist/2,
             hole_dist/2, 0])
    fasten_holes_rect(nx, ny, hole_dist, hole_r, hole_h);
}

module fasten_plate() {
  union()
    difference() {
      cube([fasten_plate_l, fasten_plate_w, fasten_plate_h]);

      translate([no_bend_beam_w + wall_w,
                 no_bend_beam_w + wall_w, -epsilon])
          import("fasten_holes_8x10_tst1.stl", convexity=4);

      // square hole for belt:
      translate([no_bend_beam_w + stepper_w,
                 no_bend_beam_w + fasten_plate_hole_dist + 2*wall_w, -epsilon])
        cube([fasten_plate_l, stepper_w - 2*wall_w, fasten_plate_h + 2*epsilon]);

      // square hole2 for prof2020 connector nuts:
      translate([2*no_bend_beam_w,
                 fasten_plate_w - no_bend_beam_w - wall_w -
                   prof2020_fasten_plate_l - profile2020_w, -epsilon])
        union() {
          cube([fasten_plate_l - 4*no_bend_beam_w,
                prof2020_fasten_plate_l/2,
                fasten_plate_h + 2*epsilon]);
          translate([0, prof2020_fasten_plate_l/2 + profile2020_w, 0])
            cube([fasten_plate_l - 4*no_bend_beam_w,
                  prof2020_fasten_plate_l/2,
                  fasten_plate_h + 2*epsilon]);
        }
    }

    cube([fasten_plate_l, no_bend_beam_w, no_bend_beam_w]);
    cube([no_bend_beam_w, fasten_plate_w, no_bend_beam_w]);
    translate([0, fasten_plate_w - no_bend_beam_w, 0])
          cube([fasten_plate_l, no_bend_beam_w, no_bend_beam_w]);
    translate([fasten_plate_l - no_bend_beam_w, 0, 0])
          cube([no_bend_beam_w, fasten_plate_w, no_bend_beam_w]);
}


module simple_fasten_plate(nx, ny, height)
{
  difference() {
    cube([(nx + 0.5) * fasten_plate_hole_dist,
          (ny + 0.5)* fasten_plate_hole_dist, height]);

    translate([fasten_plate_hole_dist/2,
               fasten_plate_hole_dist/2, -epsilon])
      fasten_holes_interleave(nx, ny, fasten_plate_hole_dist, M3hole_d/2, height + 2*epsilon);
  }
}


module stepper_holder() {
  difference() {
    union() {
      translate([0, 0, (stepper_w + wall_w)/2])
        cube([stepper_w + 2*wall_w,
              stepper_holder_l,
              stepper_w + wall_w], center = true);

      translate([0, 0, wall_w])
        cube([stepper_w + 8*wall_w, stepper_holder_l,
              2*wall_w], center = true);
    }

    translate([0, 0, stepper_w/2 - epsilon])
      cube([stepper_w,
            stepper_holder_l + 2*epsilon,
            stepper_w + epsilon], center = true);

    translate([-3*fasten_plate_hole_dist, 0, -epsilon])
      fasten_holes_rect(2, 1, 6*fasten_plate_hole_dist,
                        M3hole_d/2, 2*wall_w + 2*epsilon);
  }
}

module prof2020_holder_to_table() {
  difference() {
    union() {
      translate([0, 0, (profile2020_w + wall_w)/2])
        cube([profile2020_w + 2*wall_w,
              stepper_holder_l,
              profile2020_w + wall_w], center = true);

      translate([0, 0, 0.75*wall_w])
        cube([profile2020_w + 9*wall_w, stepper_holder_l,
              1.5*wall_w], center = true);
    }

    translate([0, 0, profile2020_w/2 - 3*epsilon])
      cube([profile2020_w,
            stepper_holder_l + 2*epsilon,
            profile2020_w - 4*epsilon], center = true);

    translate([-2*fasten_plate_hole_dist, 0, -epsilon])
      fasten_holes_rect(2, 1, 4*fasten_plate_hole_dist,
                        M4hole_d/2, 2*wall_w + 2*epsilon);
  }
}


module rod_holder_to_table() {
  difference() {
    union() {
      block_h = 30;
      block_w = 22;
      translate([-5, 0, block_h/2])
        cube([ block_w,
              //profile2020_w + 2*wall_w,
              stepper_holder_l,
              block_h], center = true);

      translate([3, 0, 0.75*wall_w])
        cube([profile2020_w + 9*wall_w, stepper_holder_l,
              1.5*wall_w], center = true);
    }

    translate([-2*fasten_plate_hole_dist, 0, -epsilon])
      fasten_holes_rect(2, 1, 3+4*fasten_plate_hole_dist,
                        M4hole_d/2, 2*wall_w + 2*epsilon);
    
    translate([-6,0,20])
        rotate([90,0,0])
            cylinder(r = rod_holder_in_r, h = stepper_holder_l);
  }
  
}

module linear_bearing_holder(ring_h)
{
  in_r = linear_bearing_out_d/2 + linear_bearing_holder_tol;
  out_r = linear_bearing_holder_out_r;

  difference() {
    ring(in_r, out_r, ring_h);

    translate([0, 0, -epsilon])
      cube([out_r + linear_bearing_holder_gap_w, // make sure it penetrates the ring
            linear_bearing_holder_gap_w,
            ring_h + 2*epsilon]);
  }
}

module camera_box() {
    
   box_depth = 13;
   box_dist = 40;
   box_h = linear_bearing_h*2 +4;
    //-- Beveled cube (4 points resolution)
  
  dist_x = 20/2;
  dist_y = 35/2;
  translate([0,box_depth,box_h/2]) 

   rotate([90,0,0])
    make_nm_holes(M3hole_d/2, dist_x, dist_y, -box_dist/2+dist_x, -box_h/2+dist_y/2-3, 3, 3) 
    translate([0,0, box_depth/2])
        rotate([90,0,0])
  difference() {
     // translate([0,0,box_depth]) 
      bcube([box_dist,box_depth,box_h],cr=4, cres=4);
      bcube([box_dist-wall_w,box_depth-wall_w,box_h+1],cr=4, cres=4);
  }
}

module counter_weight_cart()
{
  cart_h = linear_bearing_h;
  turn_axis_w = wall_w;
  side_blocks_w = 2*wall_w + turn_axis_w;
  side_blocks_l = 2*linear_bearing_holder_out_r + 4*wall_w;

  echo("side_blocks_l = ", side_blocks_l);

  rotate([0, 0, 90])
    linear_bearing_holder(cart_h);

  difference() {
    translate([-side_blocks_l/2,
              -side_blocks_w/2, cart_h/2 - side_blocks_w/2])
      cube([side_blocks_l, side_blocks_w, side_blocks_w]);

    cylinder(r = linear_bearing_out_d/2, h = cart_h, $fn = 50);
    
    translate([0, 0, cart_h/2])
      X_cylinder(turn_axis_w/2 + 2*pole_turn_tol,
                   side_blocks_l + 2*epsilon);
  }

  belt_holder_l = side_blocks_l + 2*(slider_tol + turn_axis_w);
  
  belt_holder_w = 
    sqrt(pow(cart_h/2, 2)
          + pow(linear_bearing_holder_out_r,2))
    + 2*turn_axis_w;
  
  translate([0, 0, cart_h/2]) {
    difference() {
      X_cylinder(turn_axis_w/2, belt_holder_l);

      cube([side_blocks_l - side_blocks_w, side_blocks_w, side_blocks_w], center = true);
    }
    
    translate([0, -belt_holder_w/2, 0]) {
      translate([-belt_holder_l/2 + turn_axis_w/2, 0, 0])
        Y_cylinder(turn_axis_w/2, belt_holder_w);
      translate([+belt_holder_l/2 - turn_axis_w/2, 0, 0])
        Y_cylinder(turn_axis_w/2, belt_holder_w);
    }

    translate([0, -belt_holder_w, 0]) {
      X_cylinder(turn_axis_w/2, belt_holder_l);

      translate([-pulley_belt_width - wall_w/2, 0, 0])
        X_cylinder(1.5*turn_axis_w, wall_w);
      translate([+pulley_belt_width + wall_w/2, 0, 0])
        X_cylinder(1.5*turn_axis_w, wall_w);
    }
  }
}

module camera_cart()
{

  cart_h = 2*linear_bearing_h + wall_w;
  
  translate([camera_rod_centers_dist/2, 0, 0])
    linear_bearing_holder(cart_h);

  translate([-camera_rod_centers_dist/2, 0, 0])
    rotate([0, 0, 180])
      linear_bearing_holder(cart_h);
  
  // floor between bearings on right side
  //translate([camera_rod_centers_dist/2 - linear_bearing_out_d/2, -2*wall_w, cart_h/2 - wall_w/2])
    //cube([wall_w/2, 4*wall_w, wall_w]);
  
  horis_beam_w = 1.2*wall_w;
  horis_beam_h = 2.5*wall_w;
  difference() {
    union() {
    
    translate([0,linear_bearing_holder_out_r/2+0.5,0])   
    camera_box();
      // squate horisontal holder beams
      translate([-camera_rod_centers_dist/2, - linear_bearing_holder_out_r, 0])
        cube([camera_rod_centers_dist, horis_beam_w, horis_beam_h]);
      translate([-camera_rod_centers_dist/2, 
                 -linear_bearing_holder_out_r,
                 cart_h - horis_beam_h])
        cube([camera_rod_centers_dist, horis_beam_w, horis_beam_h]);
        
      // outer beams
        translate([-camera_rod_centers_dist/2, 
                 linear_bearing_holder_out_r-horis_beam_w,
                 cart_h - horis_beam_h])
        cube([camera_rod_centers_dist, horis_beam_w, horis_beam_h]);
        
        
       translate([-camera_rod_centers_dist/2, 
                 linear_bearing_holder_out_r-horis_beam_w,
                 0])
        cube([camera_rod_centers_dist, horis_beam_w, horis_beam_h]);
      
  //    // circular beam for belt:
    //  translate([0, 0, cart_h/2])
      //  X_cylinder(horis_beam_w/2, camera_rod_centers_dist);
  
        
    
    color("red")
    translate([-belt_width/2,horis_beam_w-linear_bearing_holder_out_r-epsilon,cart_h - horis_beam_h])
    {
        GT2_attachment(horis_beam_h);

    }
    
    color("red")
    translate([-belt_width/2,horis_beam_w-linear_bearing_holder_out_r-epsilon,0])
    GT2_attachment(horis_beam_h);
    
    }
    
    color("green")
        translate([0,0, cart_h - horis_beam_h/2])
          cart_GT2_holes(30);
    
    color("green")
       translate([0,0, horis_beam_h/2])
         cart_GT2_holes(30);
    
    // linear_bearing_holder holes:
    translate([camera_rod_centers_dist/2, 0, -epsilon])
      cylinder(r = linear_bearing_out_d/2,
               h = cart_h + 2*epsilon, $fn = 50);
    translate([-camera_rod_centers_dist/2, 0, -epsilon])
      cylinder(r = linear_bearing_out_d/2,
               h = cart_h + 2*epsilon, $fn = 50);
  }
}

belt_width = 8; 
cart_belt_screw_dist = 5;
belt_buldge = 1;
module GT2_attachment(hight) {

        
        color("red")
        
  translate([0,belt_buldge,hight-1])
  rotate([0,90,0])
  belting("straight","GT2_2mm", belt_length = hight, belting_width = belt_width );	// 13 seconds
  cube([belt_width, belt_buldge,hight]);
    
}

real_dist = cart_belt_screw_dist+belt_width+M3hole_d/2;



module belt_fasten() {

    
  difference() {
       cube([real_dist+2*wall_w-1,belt_width,wall_w],center=true);
       translate([0,0,5])
       rotate([90,0,0]) 
       cart_GT2_holes(20);
      
          
      
      translate([-real_dist/2,0,wall_w/2-M3_nut_depth+epsilon])
      cylinder(r=M3_nutR, h= M3_nut_depth, $fn=6);
        
        
      translate([real_dist/2,0,wall_w/2-M3_nut_depth+epsilon])
      cylinder(r=M3_nutR, h= M3_nut_depth, $fn=6);
  }
}


module cart_GT2_holes(depth=3) {
    


    rotate([90,0,0])
    translate([-real_dist/2,0,0])
    fasten_holes_rect(2, 1, real_dist,
                            M3hole_d/2, depth + 2*epsilon);
    
}

// testing:

//color("black") rod_holder_ring();

//fasten_holes_interleave(10, 10, fasten_plate_hole_dist, M3hole_d/2, fasten_plate_h + 2*epsilon);

//color("black") linear_bearing_holder(linear_bearing_h+wall_w);



// results:

//color("blue") prof2020_holder();

//color("green") rod_holder2();

// generate separately file "fasten_holes_8x10.stl":
//fasten_holes_interleave(8, 10, fasten_plate_hole_dist, M3hole_d/2, fasten_plate_h + 2*epsilon);

// color("black") fasten_plate();

//color("red") stepper_holder();

// color("white") prof2020_holder_to_table();

 //color("blue") 
 //mirror(v= [0, 1, 0] ) 
 //rod_holder_to_table() ;

//color("black") simple_fasten_plate(8, 8, wall_w);

//color("green") linear_bearing_holder_tst(linear_bearing_holder_tol);

//color("black") 
camera_cart();
//belt_fasten();



//color("black") counter_weight_cart();
