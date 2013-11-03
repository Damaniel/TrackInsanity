/****************************************************************************
*
* Dialog.cs - Code to create, render, and handle input for basic dialog
*             boxes.
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
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;

namespace TrackInsanity.Source
{
    class Dialog
    {
        /* Element positions are defined relative to the upper left corner of the 'base' element of the dialog */
        private class Element
        {
            /* X and Y position on the dialog */
            public int x;
            public int y;

            /* Width and height */
            public int width;
            public int height;           

            /* The texture representing the button content */
            public Texture2D texture;

            /* Determines whether the button is displayed and whether it accepts clicks or not */
            public bool enabled;

            /* An identifier used to find a particular element of a dialog */
            public string name;

            /* If this is set to true, the element is a 'virtual' element -- a clickable area that has
             * no matching drawable area.  If the initial dimensions used when the element is created
             * are set to Dialog.VIRTUAL_ELEMENT, then the element will automatically be set as a 
             * virtual one.  The setVirtual() and isVirtual() methods can get or set the state of an already
             * created element. */
            public bool virtualElement;

            /* If this is a button, then these 4 values represent the min/max X and Y of the clickable area */
            public int minClickX;
            public int minClickY;
            public int maxClickX;
            public int maxClickY;

            public Element(int x, int y, int width, int height, Texture2D texture)
            {
                this.x = x;
                this.y = y;
                this.width = width;
                this.height = height;
                this.texture = texture;
                enabled = true;
                name = "";

                minClickX = -1;
                minClickY = -1;
                maxClickX = -1;
                maxClickY = -1;
            }
        }

        public const int VIRTUAL_ELEMENT = -1;

        private List<Element> elements;
        private int currentAlpha;

        private bool isVisible;

        private int xPos;
        private int yPos;

        private bool displayJustRequested;
        private bool hideJustRequested;

        private bool inTransition;

        public Dialog(int x, int y)
        {
            elements = new List<Element>();
            currentAlpha = 100;
            this.xPos = x;
            this.yPos = y;
            displayJustRequested = false;
            hideJustRequested = false;
            inTransition = false;
            isVisible = false;
        }

        public void reset()
        {
            displayJustRequested = false;
            hideJustRequested = false;
            inTransition = false;
            isVisible = false;
        }

        public void changePosition(int x, int y)
        {
            xPos = x;
            yPos = y;
        }

        public void addElement(int x, int y, int width, int height, Texture2D texture, string name, bool enabled)
        {
            Element e = new Element(x, y, width, height, texture);
            e.name = name;
            e.enabled = enabled;
            if (x == VIRTUAL_ELEMENT || y == VIRTUAL_ELEMENT || width == VIRTUAL_ELEMENT || height == VIRTUAL_ELEMENT)
            {
                e.virtualElement = true;
                e.texture = null;
            }
            else
            {
                e.virtualElement = false;
            }
            elements.Add(e);
        }

        public void addVirtualElement(string name, bool enabled)
        {
            addElement(VIRTUAL_ELEMENT, VIRTUAL_ELEMENT, VIRTUAL_ELEMENT, VIRTUAL_ELEMENT, null, name, enabled);
        }

        public bool isVirtual(string name)
        {
            for (int counter = 0; counter < elements.Count; counter++)
            {
                if (elements[counter].name == name)
                {
                    return elements[counter].virtualElement;
                }
            }
            return false;
        }

        public void setVirtual(string name, bool virtualStatus)
        {
            for (int counter = 0; counter < elements.Count; counter++)
            {
                if (elements[counter].name == name)
                {
                    elements[counter].virtualElement = virtualStatus;
                    elements[counter].texture = null;
                }
            }
        }

        public void addButtonExtents(string name, int minX, int minY, int maxX, int maxY)
        {
            for (int counter = 0; counter < elements.Count; counter++)
            {
                if (elements[counter].name == name)
                {
                    elements[counter].minClickX = minX;
                    elements[counter].minClickY = minY;
                    elements[counter].maxClickX = maxX;
                    elements[counter].maxClickY = maxY;
                    return;
                }
            }
            System.Diagnostics.Debug.WriteLine("Error while adding clickable area: Unable to find a dialog element called '{0}'.", name);
        }

        /*
         * Returns true if the specified x and y positions on screen are within the specified dialog element (but only if the element
         * is currently enabled) */
        public bool isClicked(string name, int x, int y)
        {
            bool foundElement = false;
            int counter = 0;

            /* Don't process input while the dialog is transitioning */
            if (inTransition == true)
            {
                return false;
            }

            /* Don't process input if the parent is hidden */
            if (isVisible == false)
            {
                return false;
            }

            while (foundElement == false && counter < elements.Count)
            {
                if (elements[counter].name == name)
                {
                    foundElement = true;
                }
                else
                {
                    counter++;
                }
            }

            if (foundElement == false)
            {
                System.Diagnostics.Debug.WriteLine("Unable to find element '{0}' while looking for clicked area", name);
                return false;
            }

            if (elements[counter].enabled != true)
            {
                System.Diagnostics.Debug.WriteLine("Current element '{0}' is disabled", name);
                return false;
            }

            if (x >= (xPos + elements[counter].minClickX) &&
                x <= (xPos + elements[counter].maxClickX) &&
                y >= (yPos + elements[counter].minClickY) &&
                y <= (yPos + elements[counter].maxClickY))
            {
                return true;
            }

            return false;
        }

        public void modifyAlpha(int alpha)
        {
            currentAlpha = alpha;
        }

        public void setElementVisibility(string name, bool setVisible)
        {
            for(int counter=0;counter<elements.Count; counter++)
            {
                if (elements[counter].name == name)
                {
                    elements[counter].enabled = setVisible;
                    return;
                }
            }
            System.Diagnostics.Debug.WriteLine("Error while setting element visible: Unable to find a dialog element called '{0} in {1} elements'.", name, elements.Count);
        }

        public void updateTransition(int quantity)
        {
            if (displayJustRequested == true)
            {
                currentAlpha += quantity;
                if (currentAlpha >= 100)
                {
                    currentAlpha = 100;
                    inTransition = false;
                    displayJustRequested = false;              
                }
            }

            if (hideJustRequested == true)
            {
                currentAlpha -= quantity;
                if (currentAlpha <= 0)
                {
                    currentAlpha = 0;
                    inTransition = false;
                    hideJustRequested = false;
                    isVisible = false;
                }
            }
        }

        public void setVisible(bool status)
        {
            isVisible = status;
        }

        public void processShow()
        {
            displayJustRequested = true;
            hideJustRequested = false;
            inTransition = true;
            currentAlpha = 0;
            isVisible = true;
        }

        public void processHide()
        {
            hideJustRequested = true;
            displayJustRequested = false;
            inTransition = true;
            currentAlpha = 100;

        }

        public void draw(Render r)
        {
            if (isVisible == false)
            {
                return;
            }

            if (inTransition == true)
            {
                updateTransition(25);
            }
            foreach (Element e in elements)
            {
                if (e.enabled == true && e.virtualElement == false && e.texture != null)
                {
                    r.blitAlphaSurface(0, 0, (xPos + e.x), (yPos + e.y), e.width, e.height, currentAlpha, e.texture);
                }
            }
        }
    }
}
