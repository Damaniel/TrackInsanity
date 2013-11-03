/****************************************************************************
*
* RNG.cs - a factory class that generates random ints and floats.
*
* Copyright 2007-2010 Shaun Brandt / Holy Meatgoat Software
*     <damaniel@damaniel.org>
*
* This file is part of TrackInsanity.
*
* TrackInsanity is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* TrackInsanity is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TrackInsanity; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
****************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TrackInsanity.Source
{
    public static class RNG
    {
        /* Random seed */
        private static Random random = new Random();

        public static int GetRandomInt(int min, int max)
        {
            return (int)random.Next(min, max + 1);
        }

        public static float GetRandomFloat(float min, float max)
        {
            return min + (float)random.NextDouble() * ((max) - min);
        }
    }
}
