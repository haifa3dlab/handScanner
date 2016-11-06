include <params.scad>

$fn=100;

eps = 0.05;
bigSize = 10;


//GT2
P = 2;
R1 = 0.15;
R2 = 1.00;
R3 = 0.555;
b = 0.40;
H = 1.38;
h = 0.75;
i = 0.63;

/*
// GT3
P = 3;
R1 = 0.25;
R2 = 1.52;
R3 = 0.85;
b = 0.61;
H = 2.40;
h = 1.14;
i = 1.26;
*/

// lego
h1 = 7.8;
h2 = 0.7;
r1 = 23.4/2;
r2 = 4.8/2;
r3 = 6.1/2;
pitch = 8.0;
hHub = h1;
axelHoleRadius = r2;
axelHoleLength = 2*axelHoleRadius;
axelHoleWidth = 2.3;
axelInnerRadius = 3.6/2;
minRadiusFancyHole = 1.5;



function calcX() = R1/tan(asin(R1/(R1+R2)));


module drawCircleMaskFlatR2()
{
    translate([0,R3]) difference()
    {
        union()
        {   
            translate([-bigSize/2,-bigSize]) square([bigSize,bigSize]);
        }
        union()
        {
            translate([b,h-R3]) circle(R2);            
            translate([R3+eps,-bigSize/2]) square([bigSize,bigSize]);
            translate([eps,-bigSize/2]) square([bigSize,bigSize]);
        }
    }
}


module drawCircleMaskFlatR3()
{
    translate([0,R3]) difference()
    {
        union()
        {   
            translate([-bigSize/2,-bigSize]) square([bigSize,bigSize]);
        }
        union()
        {
            circle(R3);
            translate([R3+eps,-bigSize/2]) square([bigSize,bigSize]);
            translate([eps,-bigSize/2]) square([bigSize,bigSize]);
            // approximation
            translate([-bigSize/2,-R3/2]) square([bigSize,bigSize]);                        
        }
    }
}


module drawBeltFlatHalf()
{
    difference()
    {
        union()
        {
            translate([-eps-P/2,0]) square([P/2+2*eps,H]);
        }
        union()
        {
            translate([-bigSize/2,H]) square([bigSize,bigSize]);
            translate([eps,-bigSize/2]) square([bigSize,bigSize]);
            translate([-eps-P/2-bigSize,-bigSize/2]) square([bigSize,bigSize]);
            drawCircleMaskFlatR2();
            drawCircleMaskFlatR3();
            
            x = calcX();
           translate([-x+b,H-i-R1]) circle(R1);
            translate([-bigSize-x+b,-bigSize+h]) square([bigSize,bigSize]);            
        }
    }
}


module drawBeltFlat()
{
    drawBeltFlatHalf();
    mirror([1,0,0]) drawBeltFlatHalf();
}


module drawTimingBeltUnit(prmBeltWidth) 
{
    linear_extrude(height = prmBeltWidth, convexity = 10, twist = 0) drawBeltFlat();
}


module drawTimingBelt(IsByLength, dimension)
{
    belt_length = IsByLength ? dimension : dimension * PI;
    nr_of_units = round(belt_length / P);
    corrected_belt_length = nr_of_units * P;
    corrected_belt_diameter = corrected_belt_length / PI;
    echo("nr_of_units=", nr_of_units);
    echo("corrected_belt_length=", corrected_belt_length);
    echo("corrected_belt_diameter", corrected_belt_diameter);
    for (nr = [0:nr_of_units-1])
    {
        angle = nr/nr_of_units*360;        
        rotate([0,0,angle]) translate([0,corrected_belt_diameter/2-h-i/2,0]) drawTimingBeltUnit(belt_width);
    }
}


module drawTimingBeltByLength(length)
{
    drawTimingBelt(true, length);
}    


module drawTimingBeltByDiameter(diameter)
{
    drawTimingBelt(false, diameter);
}    


module drawHole()
{
	translate([0,0,-eps]) cylinder(h2+eps, r3, r3);
	translate([0,0,h2-eps]) cylinder(h1-2*h2+2*eps, r2, r2);
	translate([0,0,h1-h2]) cylinder(h2+eps, r3, r3);
}


module drawAxelHole(prmHeight)
{	
	difference()
	{
		union()
		{
			translate([-axelHoleLength/2,-axelHoleWidth/2,-eps]) cube([axelHoleLength, axelHoleWidth, prmHeight+2*eps]);
			rotate([0,0,90]) translate([-axelHoleLength/2,-axelHoleWidth/2,-eps]) cube([axelHoleLength, axelHoleWidth, prmHeight+2*eps]);
			translate([0,0,-eps]) cylinder(prmHeight+2*eps, axelInnerRadius, axelInnerRadius);
		}
		union()
		{
			difference()
			{
				translate([0,0,-2*eps]) cylinder(prmHeight+4*eps, bigSize, bigSize);
				translate([0,0,-2*eps]) cylinder(prmHeight+4*eps, axelHoleRadius, axelHoleRadius);
			}
		}
	}
}


module drawFancyHole(radius)
{
	translate([0,0,-eps]) cylinder(h1+2*eps, radius, radius);
}


module drawHub(IsDrivePulley,diameter)
{
	difference()
	{
		union()
		{
			cylinder(hHub, diameter/2, diameter/2);
		}
		union()
		{
            minR = IsDrivePulley ? pitch+r2 : axelHoleRadius;
            nrOfHoles = IsDrivePulley ? 8 : 6;
			translate([0,0,-eps]) drawAxelHole(hHub+2*eps);
            if (IsDrivePulley)
            {
                rotate([0,0,0]) translate([0,pitch,0]) drawHole();
                rotate([0,0,90]) translate([0,pitch,0]) drawHole();
                rotate([0,0,180]) translate([0,pitch,0]) drawHole();
                rotate([0,0,270]) translate([0,pitch,0]) drawHole();
            }
/*            radiusFancyHole = 0.5*(diameter/2 - minR)/2;
            if (radiusFancyHole > minRadiusFancyHole)
            {
                for (nr = [0:nrOfHoles-1])
                {
                    angle = nr/nrOfHoles*360;                
                    rotate([0,0,angle]) translate([0,(minR+diameter/2)/2,0]) drawFancyHole(radiusFancyHole);
                }   
            }
            */
		}
	}
}

module drawPulley(diameter, IsDrivePulley=false)
{
    nr_of_units = round(diameter * PI / P);
    corrected_diameter = nr_of_units * P / PI;
    echo("nr_of_units=", nr_of_units);
    echo("corrected_diameter=", corrected_diameter);
    
    difference()
    {
        union()
        {
            drawHub(IsDrivePulley,diameter);
        }
        
        union()
        {
            zPosBelt = (hHub-pulley_belt_width)/2;
            for (nr = [0:nr_of_units-1])
            {
                angle = nr/nr_of_units*360;                
                rotate([0,0,angle]) translate([0,corrected_diameter/2-h-i/2,zPosBelt]) drawTimingBeltUnit(pulley_belt_width);
            }
        }
        
    }    
}


module drawDrivePulley(diameter)
{
    drawPulley(diameter,true);
}    


// test
//drawBeltFlat();
//drawCircleMaskFlatR2();
//drawCircleMaskFlatR3();
//drawTimingBeltUnit(); 


// parts
//drawTimingBeltByLength(250);
//drawTimingBeltByDiameter(75);
drawDrivePulley(100);
//drawPulley(100, false);


