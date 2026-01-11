using bridge.Configuration;
using bridge.Template;
using Reloaded.Hooks.ReloadedII.Interfaces;
using Reloaded.Mod.Interfaces;
using System.Runtime.InteropServices;

namespace bridge
{
    /// <summary>
    /// Your mod logic goes here.
    /// </summary>
    public class Mod : ModBase // <= Do not Remove.
    {
        /// <summary>
        /// Provides access to the mod loader API.
        /// </summary>
        private readonly IModLoader _modLoader;

        /// <summary>
        /// Provides access to the Reloaded.Hooks API.
        /// </summary>
        /// <remarks>This is null if you remove dependency on Reloaded.SharedLib.Hooks in your mod.</remarks>
        private readonly IReloadedHooks? _hooks;

        /// <summary>
        /// Provides access to the Reloaded logger.
        /// </summary>
        private readonly ILogger _logger;

        /// <summary>
        /// Entry point into the mod, instance that created this class.
        /// </summary>
        private readonly IMod _owner;

        /// <summary>
        /// Provides access to this mod's configuration.
        /// </summary>
        private Config _configuration;

        /// <summary>
        /// The configuration of the currently executing mod.
        /// </summary>
        private readonly IModConfig _modConfig;

        [StructLayout(LayoutKind.Sequential)]
        public struct ConfigC
        {
            [MarshalAs(UnmanagedType.I1)] public bool TitleScr;
            [MarshalAs(UnmanagedType.I1)] public bool DemoMode;
            public int TDarkChaosEmeEnum;
            [MarshalAs(UnmanagedType.I1)] public bool IndirectOFF;
            [MarshalAs(UnmanagedType.I1)] public bool Exit;
        }

        [DllImport("sh-fixed-edition.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitMod(ref ConfigC config);

        public Mod(ModContext context)
        {
            _modLoader = context.ModLoader;
            _hooks = context.Hooks;
            _logger = context.Logger;
            _owner = context.Owner;
            _configuration = context.Configuration;
            _modConfig = context.ModConfig;


            var config = new ConfigC
            {
                TitleScr = _configuration.TitleScr,
                DemoMode = _configuration.DemoMode,
                TDarkChaosEmeEnum = 0,
                IndirectOFF = _configuration.IndirectOFF,
                Exit = _configuration.Exit
            };
            InitMod(ref config);

            //!
        }

        #region Standard Overrides
        public override void ConfigurationUpdated(Config configuration)
        {
            /*
            if (_configuration.TitleScr != configuration.TitleScr)
                Config_TitleScr(configuration.TitleScr);

            if (_configuration.DemoMode != configuration.DemoMode)
                Config_DemoMode(configuration.DemoMode);

            Config_TDarkChaosEme(configuration.TDarkChaosEmeEnum);
            _configuration = configuration;

            if (_configuration.IndirectOFF != configuration.IndirectOFF)
                Config_IndirectOFF(configuration.IndirectOFF);

            if (_configuration.Exit != configuration.Exit)
                Config_DemoMode(configuration.Exit);
            */
            _configuration = configuration;
            _logger.WriteLine($"SH Fixed Edition configuration updated: Please restart the game for the changes to take effect.");
        }
        #endregion

        #region For Exports, Serialization etc.
#pragma warning disable CS8618 // Non-nullable field must contain a non-null value when exiting constructor. Consider declaring as nullable.
        public Mod() { }
#pragma warning restore CS8618
        #endregion
    }
}