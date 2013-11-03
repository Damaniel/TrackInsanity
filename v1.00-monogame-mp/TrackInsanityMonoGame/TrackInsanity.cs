/****************************************************************************
*
* TrackInsanity.cs - The main game loop.
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
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input.Touch;

namespace TrackInsanity.Source
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class TrackInsanity : Microsoft.Xna.Framework.Game
    {
        public static readonly int OK = 1;
        public static readonly int ERROR = 0;

        public static readonly int TRUE = 1;
        public static readonly int FALSE = 0;

        /* Minimum and maximum player count */
        public static int MIN_PLAYERS = 2;
        public static int MAX_PLAYERS = 6;

        /* The game, and the render instance that will be used to display it */
        private Game GameInstance;
        private Render RenderInstance;

        /* Information about the touch screen */
        private TouchPanelCapabilities panelCaps;

        /* XNA objects used to draw items onto the screen */
        public GraphicsDeviceManager graphics;
        private SpriteBatch spriteBatch;

        /* This matrix is used to rotate an 800x480 display to fit on a 480x800 screen. */
        private Matrix projection;

        public TrackInsanity()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            graphics.PreferredBackBufferWidth = 800;
            graphics.PreferredBackBufferHeight = 480;

            SharedData.xRes = graphics.PreferredBackBufferWidth;
            SharedData.yRes = graphics.PreferredBackBufferHeight;

            SharedData.graphics = graphics.GraphicsDevice;

#if WINDOWS
            /* Show the mouse cursor */
            IsMouseVisible = true;
#endif

#if WINDOWS
            // Frame rate is 60 fps by default for Windows.
            TargetElapsedTime = TimeSpan.FromSeconds(1 / 30.0);
#else
            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromSeconds(1 / 30.0);
#endif 
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            GameInstance = new Game();

            RenderInstance = new Render();
            RenderInstance.setGameInstance(GameInstance);

            /* Reset the shared data structure */
            SharedData.reset();
 
            graphics.IsFullScreen = false;
            panelCaps = TouchPanel.GetCapabilities();

            projection = Matrix.CreateTranslation(new Vector3(1f, 1f, 0f)) * 
                         Matrix.CreateRotationZ(1.570795f) * 
                         Matrix.CreateScale(new Vector3(1f, 1f, 0.0f)) *
                         Matrix.CreateTranslation(new Vector3(481, -1, 0));

            /* Start the frame counter */
            SharedData.numFrames = 0;

            base.Initialize();
        }


        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);
            RenderInstance.setSpriteBatch(spriteBatch);
            
            RenderInstance.loadContent(this.Content);
            RenderInstance.createDialogs();

            /* Set the state machine to its initial state (rendering the title screen) */
            RenderInstance.setInitialRenderState();                      
        }

        protected override void UnloadContent()
        {
            Content.Unload();
        }


        protected override void Update(GameTime gameTime)
        {
            if (SharedData.exitGame == TrackInsanity.TRUE)
            {
                this.Exit();
            }


            SharedData.curTicks = (int)gameTime.TotalGameTime.TotalMilliseconds;

            RenderInstance.updateLogic();
#if WINDOWS
            RenderInstance.processInput();
#else
            RenderInstance.processInput(panelCaps);
#endif
            RenderInstance.checkTimingConditions();

            SharedData.numFrames++;

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {        
#if WINDOWS || XBOX
            spriteBatch.Begin();
#else
            spriteBatch.Begin(SpriteSortMode.Deferred, BlendState.AlphaBlend, SamplerState.LinearClamp, DepthStencilState.Default, RasterizerState.CullNone, null, projection);
#endif
            RenderInstance.updateScreen(graphics.GraphicsDevice);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
