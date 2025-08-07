using Microsoft.Maui.Graphics;
using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GnollHackX
{

    // Enums for particle system
    public enum ParticleType
    {
        Circle,
        Star,
        FourPointedStar,
        Image
    }

    public enum BlendMode
    {
        Normal,
        Add,
        Multiply,
        Screen
    }

    // Vector2 struct for 2D operations
    public struct Vector2
    {
        public float X, Y;

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public static Vector2 operator +(Vector2 a, Vector2 b) => new Vector2(a.X + b.X, a.Y + b.Y);
        public static Vector2 operator -(Vector2 a, Vector2 b) => new Vector2(a.X - b.X, a.Y - b.Y);
        public static Vector2 operator *(Vector2 a, float scalar) => new Vector2(a.X * scalar, a.Y * scalar);
        public static Vector2 operator /(Vector2 a, float scalar) => new Vector2(a.X / scalar, a.Y / scalar);

        public float Length() => (float)Math.Sqrt(X * X + Y * Y);
        public Vector2 Normalized() => this / Length();
        public static Vector2 Zero => new Vector2(0, 0);
    }

    // Individual particle class
    public class Particle
    {
        public Vector2 Position;
        public Vector2 Velocity;
        public Vector2 Acceleration;
        public float Life;
        public float MaxLife;
        public float Size;
        public float StartSize;
        public float EndSize;
        public SKColor StartColor;
        public SKColor EndColor;
        public float Rotation;
        public float RotationSpeed;
        public ParticleType Type;
        public SKImage Image;
        public BlendMode BlendMode;
        public bool IsAlive => Life > 0;
        public float LifeRatio => Life / MaxLife;

        public Particle()
        {
            Position = Vector2.Zero;
            Velocity = Vector2.Zero;
            Acceleration = Vector2.Zero;
            Life = 1.0f;
            MaxLife = 1.0f;
            Size = 1.0f;
            StartSize = 1.0f;
            EndSize = 1.0f;
            StartColor = SKColors.White;
            EndColor = SKColors.White;
            Rotation = 0.0f;
            RotationSpeed = 0.0f;
            Type = ParticleType.Circle;
            BlendMode = BlendMode.Normal;
        }

        public bool IsActive { get; set; }

        public void Activate()
        {
            IsActive = true;
            // Add other logic to reset particle state
        }

        public void Deactivate()
        {
            IsActive = false;
            // Add logic to clean up or hide the particle
        }

        public void Update(float deltaTime)
        {
            if (!IsActive || !IsAlive) return;

            Velocity += Acceleration * deltaTime;
            Position += Velocity * deltaTime;
            Life -= deltaTime;
            Rotation += RotationSpeed * deltaTime;

            // Interpolate size and color based on life
            float t = 1.0f - LifeRatio;
            Size = Lerp(StartSize, EndSize, t);
        }

        public SKColor GetCurrentColor()
        {
            float t = 1.0f - LifeRatio;
            return LerpColor(StartColor, EndColor, t);
        }

        private float Lerp(float a, float b, float t) => a + (b - a) * t;

        private SKColor LerpColor(SKColor a, SKColor b, float t)
        {
            return new SKColor(
                (byte)Lerp(a.Red, b.Red, t),
                (byte)Lerp(a.Green, b.Green, t),
                (byte)Lerp(a.Blue, b.Blue, t),
                (byte)Lerp(a.Alpha, b.Alpha, t)
            );
        }
    }

    // Particle emitter configuration
    public class EmitterConfig
    {
        public Vector2 Position = Vector2.Zero;
        public float Width = 0.0f; // 0 means point emitter
        public float Height = 0.0f; // 0 means point emitter
        public float StartTime = 0.0f; // When the emitter should start (in seconds)
        public float Duration = -1.0f; // How long to emit (-1 means infinite)
        public float EmissionRate = 10.0f;
        public float ParticleLifespan = 2.0f;
        public float ParticleLifespanVariance = 0.5f;
        public Vector2 InitialVelocity = Vector2.Zero;
        public Vector2 VelocityVariance = new Vector2(50, 50);
        public Vector2 Gravity = new Vector2(0, 98);
        public float StartSize = 10.0f;
        public float EndSize = 0.0f;
        public float SizeVariance = 3.0f;
        public SKColor StartColor = SKColors.White;
        public SKColor EndColor = SKColors.Transparent;
        public float RotationSpeed = 0.0f;
        public float RotationSpeedVariance = 0.0f;
        public ParticleType Type = ParticleType.Circle;
        public BlendMode BlendMode = BlendMode.Normal;
        public SKImage ParticleImage = null;
    }

    public class Emitter
    {
        public readonly EmitterConfig Config;
        public float TimeSinceLastEmission;
        public float TotalTime; // Track total elapsed time

        public Emitter(EmitterConfig config, float timeSinceLastEmission)
        {
            Config = config;
            TimeSinceLastEmission = timeSinceLastEmission;
        }

        public void ResetTime()
        {
            TotalTime = 0.0f;
            TimeSinceLastEmission = 0.0f;
        }

        public bool IsAlive
        {
            get
            {
                if (Config.Duration > 0.0f)
                {
                    float emissionEndTime = Config.StartTime + Config.Duration;
                    if (TotalTime > emissionEndTime) return false;
                }
                return true;
            }
        }

        public bool IsEmitting
        {
            get
            {
                if (TotalTime < Config.StartTime) 
                    return false;

                return IsAlive;
            }
        }
    }

    public class ParticlePool
    {
        private readonly Queue<Particle> _pool;
        private readonly int _initialSize;

        public ParticlePool(int initialSize)
        {
            _initialSize = initialSize;
            _pool = new Queue<Particle>();

            for (int i = 0; i < _initialSize; i++)
            {
                _pool.Enqueue(new Particle());
            }
        }

        public Particle GetParticle()
        {
            if (_pool.Count > 0)
            {
                var particle = _pool.Dequeue();
                particle.Activate();
                return particle;
            }
            else
            {
                // Optionally grow the pool
                var newParticle = new Particle();
                newParticle.Activate();
                return newParticle;
            }
        }

        public void ReturnParticle(Particle particle)
        {
            particle.Deactivate();
            _pool.Enqueue(particle);
        }
    }

    // Main particle system class
    public class ParticleSystem
    {
        private List<Emitter> emitters;
        private List<Particle> particles;
        private Random random;
        private ParticlePool _particlePool = new ParticlePool(200);

        public ParticleSystem()
        {
            emitters = new List<Emitter>();
            particles = new List<Particle>();
            random = new Random();
        }

        public void AddEmitter(Emitter emitter)
        {
            emitters.Add(emitter);
        }

        public void Update(float deltaTime)
        {
            // Update existing particles
            for (int i = particles.Count - 1; i >= 0; i--)
            {
                particles[i].Update(deltaTime);
                if (!particles[i].IsAlive)
                {
                    particles.RemoveAt(i);
                }
            }

            // Emit new particles
            for (int i = emitters.Count - 1; i >= 0; i--)
            {
                var emitter = emitters[i];
                UpdateEmitter(deltaTime, emitter);
                if (!emitter.IsAlive)
                {
                    emitters.RemoveAt(i);
                }
            }
        }

        public void UpdateEmitter(float deltaTime, Emitter emitter)
        {
            emitter.TotalTime += deltaTime;

            if (!emitter.IsEmitting)
                return;
            
            // Emit new particles
            emitter.TimeSinceLastEmission += deltaTime;
            float emissionInterval = 1.0f / emitter.Config.EmissionRate;

            while (emitter.TimeSinceLastEmission >= emissionInterval)
            {
                EmitParticle(emitter.Config);
                emitter.TimeSinceLastEmission -= emissionInterval;
            }
        }

        private void EmitParticle(EmitterConfig config)
        {
            var particle = _particlePool.GetParticle();

            // Position
            Vector2 emissionPosition = config.Position;
            if (config.Width > 0 || config.Height > 0)
            {
                float offsetX = config.Width > 0 ? RandomRange(-config.Width / 2, config.Width / 2) : 0;
                float offsetY = config.Height > 0 ? RandomRange(-config.Height / 2, config.Height / 2) : 0;
                emissionPosition = new Vector2(config.Position.X + offsetX, config.Position.Y + offsetY);
            }
            particle.Position = emissionPosition;

            // Velocity
            float vx = config.InitialVelocity.X + RandomRange(-config.VelocityVariance.X, config.VelocityVariance.X);
            float vy = config.InitialVelocity.Y + RandomRange(-config.VelocityVariance.Y, config.VelocityVariance.Y);
            particle.Velocity = new Vector2(vx, vy);

            // Acceleration (gravity)
            particle.Acceleration = config.Gravity;

            // Life
            particle.MaxLife = config.ParticleLifespan + RandomRange(-config.ParticleLifespanVariance, config.ParticleLifespanVariance);
            particle.Life = particle.MaxLife;

            // Size
            particle.StartSize = config.StartSize + RandomRange(-config.SizeVariance, config.SizeVariance);
            particle.EndSize = config.EndSize;
            particle.Size = particle.StartSize;

            // Color
            particle.StartColor = config.StartColor;
            particle.EndColor = config.EndColor;

            // Rotation
            particle.Rotation = RandomRange(0, 360);
            particle.RotationSpeed = config.RotationSpeed + RandomRange(-config.RotationSpeedVariance, config.RotationSpeedVariance);

            // Type and settings
            particle.Type = config.Type;
            particle.BlendMode = config.BlendMode;
            particle.Image = config.ParticleImage;

            particles.Add(particle);
        }

        public void Render(SKCanvas canvas)
        {
            foreach (var particle in particles)
            {
                RenderParticle(canvas, particle);
            }
        }

        private SKPaint _renderPaint = new SKPaint  
        {
            IsAntialias = true,
        };  

        private void RenderParticle(SKCanvas canvas, Particle particle)
        {
            if (!particle.IsAlive) return;

            canvas.Save();
            canvas.Translate(particle.Position.X, particle.Position.Y);
            canvas.RotateDegrees(particle.Rotation);

            _renderPaint.Color = particle.GetCurrentColor();
            //_renderPaint.IsAntialias = true;

            // Set blend mode
            switch (particle.BlendMode)
            {
                case BlendMode.Add:
                    _renderPaint.BlendMode = SKBlendMode.Plus;
                    break;
                case BlendMode.Multiply:
                    _renderPaint.BlendMode = SKBlendMode.Multiply;
                    break;
                case BlendMode.Screen:
                    _renderPaint.BlendMode = SKBlendMode.Screen;
                    break;
                default:
                    _renderPaint.BlendMode = SKBlendMode.SrcOver;
                    break;
            }

            switch (particle.Type)
            {
                case ParticleType.Circle:
                    RenderCircle(canvas, _renderPaint, particle.Size);
                    break;
                case ParticleType.Star:
                    RenderStar(canvas, _renderPaint, particle.Size);
                    break;
                case ParticleType.FourPointedStar:
                    RenderFourPointedStar(canvas, _renderPaint, particle.Size);
                    break;
                case ParticleType.Image:
                    if (particle.Image != null)
                        RenderImage(canvas, _renderPaint, particle.Image, particle.Size);
                    break;
            }

            canvas.Restore();
        }

        SKMaskFilter _savedMaskFilter = null;
        private void RenderCircle(SKCanvas canvas, SKPaint paint, float size)
        {
            // Add glow effect for magic particles
            if (_savedMaskFilter == null)
            {
                _savedMaskFilter = SKMaskFilter.CreateBlur(SKBlurStyle.Normal, size * 0.1f);
            }
            paint.MaskFilter = _savedMaskFilter;
            canvas.DrawCircle(0, 0, size, paint);

            // Inner bright core
            //paint.MaskFilter?.Dispose();
            paint.MaskFilter = null;
            paint.Color = paint.Color.WithAlpha((byte)(paint.Color.Alpha * 0.8f));
            canvas.DrawCircle(0, 0, size * 0.6f, paint);
        }

        private void RenderStar(SKCanvas canvas, SKPaint paint, float size)
        {
            var path = CreateStarPath(size);

            // Outer glow
            if (_savedMaskFilter == null)
            {
                _savedMaskFilter = SKMaskFilter.CreateBlur(SKBlurStyle.Normal, size * 0.1f);
            }
            paint.MaskFilter = _savedMaskFilter;
            canvas.DrawPath(path, paint);

            // Inner star
            //paint.MaskFilter?.Dispose();
            paint.MaskFilter = null;
            canvas.DrawPath(path, paint);

            path.Dispose();
        }

        SKMaskFilter _savedMaskFilter2 = null;
        private void RenderFourPointedStar(SKCanvas canvas, SKPaint paint, float size)
        {
            var path = CreateFourPointedStarPath(size);

            // Outer glow
            if (_savedMaskFilter2 == null)
            {
                _savedMaskFilter2 = SKMaskFilter.CreateBlur(SKBlurStyle.Normal, size * 0.15f);
            }
            paint.MaskFilter = _savedMaskFilter2;
            canvas.DrawPath(path, paint);

            // Inner bright core
            //paint.MaskFilter?.Dispose();
            paint.MaskFilter = null;
            canvas.DrawPath(path, paint);

            path.Dispose();
        }

        private void RenderImage(SKCanvas canvas, SKPaint paint, SKImage image, float size)
        {
            float scale = size / Math.Max(image.Width, image.Height);
            float width = image.Width * scale;
            float height = image.Height * scale;

            var rect = new SKRect(-width / 2, -height / 2, width / 2, height / 2);
            canvas.DrawImage(image, rect, paint);
        }

        private SKPath CreateStarPath(float radius)
        {
            var path = new SKPath();
            int points = 5;
            float innerRadius = radius * 0.4f;

            for (int i = 0; i < points * 2; i++)
            {
                float angle = (float)(i * Math.PI / points);
                float currentRadius = (i % 2 == 0) ? radius : innerRadius;
                float x = (float)(Math.Cos(angle) * currentRadius);
                float y = (float)(Math.Sin(angle) * currentRadius);

                if (i == 0)
                    path.MoveTo(x, y);
                else
                    path.LineTo(x, y);
            }

            path.Close();
            return path;
        }

        private SKPath CreateFourPointedStarPath(float radius)
        {
            var path = new SKPath();
            float innerRadius = radius * 0.3f;

            // Create 4-pointed star (diamond-like with points)
            for (int i = 0; i < 8; i++)
            {
                float angle = (float)(i * Math.PI / 4); // 8 points total (4 outer, 4 inner)
                float currentRadius = (i % 2 == 0) ? radius : innerRadius;
                float x = (float)(Math.Cos(angle) * currentRadius);
                float y = (float)(Math.Sin(angle) * currentRadius);

                if (i == 0)
                    path.MoveTo(x, y);
                else
                    path.LineTo(x, y);
            }

            path.Close();
            return path;
        }
        
        private float RandomRange(float min, float max)
        {
            return (float)(random.NextDouble() * (max - min) + min);
        }

        public bool IsEmitterActive(Emitter emitter)
        {
            return emitter.IsEmitting;
        }

        public int ParticleCount => particles.Count;
    }

    // Effect presets
    public static class ParticleEffects
    {
        // Magic effect using circles with scintillation
        public static EmitterConfig CreateMagicEffect(Vector2 position)
        {
            return new EmitterConfig
            {
                Position = position,
                Width = 20.0f, // Small emission area
                Height = 20.0f,
                StartTime = 0.0f,
                Duration = -1.0f, // Infinite
                EmissionRate = 50.0f,
                ParticleLifespan = 3.0f,
                ParticleLifespanVariance = 1.0f,
                InitialVelocity = new Vector2(0, -20),
                VelocityVariance = new Vector2(80, 60),
                Gravity = new Vector2(0, -10), // Slight upward drift
                StartSize = 8.0f,
                EndSize = 0.0f,
                SizeVariance = 4.0f,
                StartColor = new SKColor(138, 43, 226, 255), // Violet
                EndColor = new SKColor(0, 191, 255, 0), // Deep sky blue, transparent
                RotationSpeed = 0.0f,
                RotationSpeedVariance = 0.0f,
                Type = ParticleType.Circle,
                BlendMode = BlendMode.Add
            };
        }

        // Power-up effect using stars
        public static EmitterConfig CreatePowerUpEffect(Vector2 position)
        {
            return new EmitterConfig
            {
                Position = position,
                EmissionRate = 25.0f,
                ParticleLifespan = 2.5f,
                ParticleLifespanVariance = 0.8f,
                InitialVelocity = new Vector2(0, -50),
                VelocityVariance = new Vector2(100, 40),
                Gravity = new Vector2(0, -20), // Strong upward movement
                StartSize = 12.0f,
                EndSize = 25.0f, // Grows over time
                SizeVariance = 3.0f,
                StartColor = new SKColor(255, 215, 0, 255), // Gold
                EndColor = new SKColor(255, 255, 255, 0), // White, transparent
                RotationSpeed = 180.0f, // Rotate
                RotationSpeedVariance = 90.0f,
                Type = ParticleType.Star,
                BlendMode = BlendMode.Add
            };
        }

        // Fire effect using images (you would need to provide fire particle images)
        public static EmitterConfig CreateFireEffect(Vector2 position, SKImage fireImage = null)
        {
            return new EmitterConfig
            {
                Position = position,
                Width = 40.0f, // Wide emission for fire base
                Height = 10.0f, // Shallow height
                StartTime = 0.0f,
                Duration = -1.0f, // Infinite
                EmissionRate = 40.0f,
                ParticleLifespan = 2.0f,
                ParticleLifespanVariance = 0.5f,
                InitialVelocity = new Vector2(0, -80),
                VelocityVariance = new Vector2(30, 20),
                Gravity = new Vector2(0, -30), // Flames rise up
                StartSize = 15.0f,
                EndSize = 5.0f,
                SizeVariance = 5.0f,
                StartColor = new SKColor(255, 69, 0, 255), // Red-orange
                EndColor = new SKColor(255, 140, 0, 0), // Dark orange, transparent
                RotationSpeed = 30.0f,
                RotationSpeedVariance = 20.0f,
                Type = fireImage != null ? ParticleType.Image : ParticleType.Circle,
                BlendMode = BlendMode.Add,
                ParticleImage = fireImage
            };
        }

        // Fast power-up effect using 4-pointed stars going straight up
        public static EmitterConfig CreateFastPowerUpEffect(Vector2 position, float startTime)
        {
            return new EmitterConfig
            {
                Position = position,
                Width = 40.0f, // Point emitter
                Height = 10.0f,
                StartTime = startTime,
                Duration = 2.0f, // Limited duration burst effect
                EmissionRate = 60.0f, // High emission rate for burst effect
                ParticleLifespan = 1.2f, // Short lifespan for quick dissipation
                ParticleLifespanVariance = 0.3f,
                InitialVelocity = new Vector2(0, -120), // Strong upward velocity
                VelocityVariance = new Vector2(15, 20), // Very little horizontal spread
                Gravity = new Vector2(0, -40), // Strong upward acceleration
                StartSize = 8.0f,
                EndSize = 20.0f, // Grows as it rises
                SizeVariance = 2.0f,
                StartColor = new SKColor(0, 255, 255, 255), // Bright cyan
                EndColor = new SKColor(255, 255, 255, 0), // White, transparent
                RotationSpeed = 360.0f, // Fast rotation
                RotationSpeedVariance = 180.0f,
                Type = ParticleType.FourPointedStar,
                BlendMode = BlendMode.Add
            };
        }

        // Example: Timed explosion effect
        public static EmitterConfig CreateExplosionEffect(Vector2 position, float startTime)
        {
            return new EmitterConfig
            {
                Position = position,
                Width = 5.0f,
                Height = 5.0f,
                StartTime = startTime,
                Duration = 0.5f, // Very short burst
                EmissionRate = 200.0f, // Extremely high emission rate
                ParticleLifespan = 1.5f,
                ParticleLifespanVariance = 0.5f,
                InitialVelocity = Vector2.Zero,
                VelocityVariance = new Vector2(150, 150), // Explode in all directions
                Gravity = new Vector2(0, 50), // Fall down after explosion
                StartSize = 12.0f,
                EndSize = 2.0f,
                SizeVariance = 4.0f,
                StartColor = new SKColor(255, 255, 0, 255), // Bright yellow
                EndColor = new SKColor(255, 0, 0, 0), // Red, transparent
                RotationSpeed = 0.0f,
                RotationSpeedVariance = 360.0f,
                Type = ParticleType.Circle,
                BlendMode = BlendMode.Add
            };
        }
    }

    public class ParticleSystemDemo
    {
        private ParticleSystem _particleSystem;
        DateTime _lastUpdateTime;

        public ParticleSystemDemo()
        {
            _particleSystem = new ParticleSystem();

            // Initialize effect configurations
            EmitterConfig magicConfig = ParticleEffects.CreateMagicEffect(new Vector2(200, 400));
            EmitterConfig powerUpConfig = ParticleEffects.CreatePowerUpEffect(new Vector2(400, 400));
            EmitterConfig fireConfig = ParticleEffects.CreateFireEffect(new Vector2(600, 400));
            EmitterConfig fastPowerUpConfig = ParticleEffects.CreateFastPowerUpEffect(new Vector2(800, 400), 5f);
            EmitterConfig fastPowerUpConfig2 = ParticleEffects.CreateFastPowerUpEffect(new Vector2(800, 400), 20f);
            EmitterConfig explosionConfig = ParticleEffects.CreateExplosionEffect(new Vector2(1000, 400), 10f);
            _particleSystem.AddEmitter(new Emitter(magicConfig, 0));
            _particleSystem.AddEmitter(new Emitter(powerUpConfig, 0));
            _particleSystem.AddEmitter(new Emitter(fireConfig, 0));
            _particleSystem.AddEmitter(new Emitter(fastPowerUpConfig, 0));
            _particleSystem.AddEmitter(new Emitter(fastPowerUpConfig2, 0));
            _particleSystem.AddEmitter(new Emitter(explosionConfig, 0));
            _lastUpdateTime = DateTime.Now;
        }

        public void Update()
        {
            DateTime now = DateTime.Now;
            float deltaTime = (float)(now - _lastUpdateTime).TotalSeconds;
            _lastUpdateTime = now;
            _particleSystem.Update(deltaTime);
        }

        public void Render(SKCanvas canvas)
        {
            // Render all particle systems
            _particleSystem.Render(canvas);
        }
    }
}
