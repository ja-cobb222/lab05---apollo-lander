/***********************************************************************
* Source File:
*    LANDER
* Author:
*    Br. Helfrich
* Summary:
*    All the information about the lunar lander
************************************************************************/
#include "lander.h"
#include "acceleration.h"
#include <cmath>

void Lander::reset(const Position& posUpperRight)
{
   angle.setUp();  
   velocity.setDX(random(-10.0, -4.0));
   velocity.setDY(random(-2.0, 2.0));
   pos.setX(posUpperRight.getX() - 1.0);
   pos.setY(random(posUpperRight.getY() * 0.75, posUpperRight.getY() * 0.95));  // Correct
   status = PLAYING;
   fuel = FUEL_MAX;
}

void Lander::draw(const Thrust& thrust, ogstream& gout) const
{
   gout.drawLander(pos, angle.getRadians());
   
   if (isFlying() && fuel > 0.0)
      gout.drawLanderFlames(pos, angle.getRadians(),
         thrust.isMain(), thrust.isClock(), thrust.isCounter());
}

Acceleration Lander::input(const Thrust& thrust, double gravity)
{
   Acceleration a;

   a.addDDY(gravity);

   if (fuel == 0.0)
      return a;

   if (thrust.isMain()) {
      double power = (LANDER_THRUST / LANDER_WEIGHT);
      a.addDDX(-sin(angle.getRadians()) * power);
      a.addDDY(cos(angle.getRadians()) * power);
      fuel -= FUEL_MAIN_THRUST;
   }

   if (thrust.isClock()) {
      angle.add(0.1);
      fuel -= FUEL_ROTATE;
   }
   if (thrust.isCounter()) {
      angle.add(-0.1);
      fuel -= FUEL_ROTATE;
   }

   if (fuel < 0.0)
      fuel = 0.0;

   return a;
}

void Lander::coast(Acceleration& acceleration, double time)
{
   // Update velocity with acceleration over time
   velocity.add(acceleration, time);

   // Update position 
   pos.add(acceleration, velocity, time);
}