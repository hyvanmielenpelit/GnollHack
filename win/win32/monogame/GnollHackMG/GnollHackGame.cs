using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Runtime.InteropServices;

namespace GnollHackMG
{

    public class GnollHackGame : Game
    {
        private GraphicsDeviceManager _graphics;
        private SpriteBatch _spriteBatch;
        private SpriteFont _spriteFont;

        [DllImport(@"GnollHackDLL.dll", CharSet = CharSet.Unicode)]
        public static extern int DoSomeCalc();

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void ProgressCallback(int value);

        [DllImport(@"GnollHackDLL.dll")]
        public static extern void DoWork([MarshalAs(UnmanagedType.FunctionPtr)] ProgressCallback callbackPointer);


        public GnollHackGame()
        {
            _graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            IsMouseVisible = true;
        }

        protected override void Initialize()
        {
            // TODO: Add your initialization logic here

            base.Initialize();
        }

        protected override void LoadContent()
        {
            _spriteBatch = new SpriteBatch(GraphicsDevice);
            _spriteFont = Content.Load<SpriteFont>("Font1");

            // TODO: use this.Content to load your game content here
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            // TODO: Add your update logic here

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            _spriteBatch.Begin();

            int fromDLLvalue = DoSomeCalc();

            // define a progress callback delegate
            ProgressCallback callback =
                (value) =>
                {
                    _spriteBatch.DrawString(_spriteFont, value.ToString(), new Vector2(10, 50), Color.White);
                };

            _spriteBatch.DrawString(_spriteFont, "Hello World!", new Vector2(10, 10), Color.White);
            _spriteBatch.DrawString(_spriteFont, fromDLLvalue.ToString(), new Vector2(10, 30), Color.White);
            DoWork(callback);

            _spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
