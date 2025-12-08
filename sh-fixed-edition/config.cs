using sh_fixed_edition.Template.Configuration;
using Reloaded.Mod.Interfaces.Structs;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;

namespace sh_fixed_edition.Configuration
{
    public class Config : Configurable<Config>
    {
        /*  (collaped configs in case i need or mess smt up lol)
            User Properties:
                - Please put all of your configurable properties here.
    
            By default, configuration saves as "Config.json" in mod user config folder.    
            Need more config files/classes? See Configuration.cs
    
            Available Attributes:
            - Category
            - DisplayName
            - Description
            - DefaultValue

            // Technically Supported but not Useful
            - Browsable
            - Localizable

            The `DefaultValue` attribute is used as part of the `Reset` button in Reloaded-Launcher.


        [DisplayName("String")]
        [Description("This is a string.")]
        [DefaultValue("Default Name")]
        public string String { get; set; } = "Default Name";

        [DisplayName("Int")]
        [Description("This is an int.")]
        [DefaultValue(42)]
        public int Integer { get; set; } = 42;

        [DisplayName("Bool")]
        [Description("This is a bool.")]
        [DefaultValue(true)]
        public bool Boolean { get; set; } = true;

        [DisplayName("Float")]
        [Description("This is a floating point number.")]
        [DefaultValue(6.987654F)]
        public float Float { get; set; } = 6.987654F;

        [DisplayName("Enum")]
        [Description("This is an enumerable.")]
        [DefaultValue(SampleEnum.ILoveIt)]
        public SampleEnum Reloaded { get; set; } = SampleEnum.ILoveIt;

        public enum SampleEnum
        {
            [Display(Name = "No Opinion 🤷")]
            NoOpinion,
            [Display(Name = "It's Sucks! 👎")]
            Sucks,
            [Display(Name = "It's mediocre 😐")]
            IsMediocre,
            [Display(Name = "It's okay! 👍")]
            IsOk,
            [Display(Name = "It's cool! 😎")]
            IsCool,
            [Display(Name = "I Love It!!! ❤️🔥")]
            ILoveIt,
        }

        [DisplayName("Int Slider")]
        [Description("This is a int that uses a slider control similar to a volume control slider.")]
        [DefaultValue(100)]
        [SliderControlParams(
            minimum: 0.0,
            maximum: 100.0,
            smallChange: 1.0,
            largeChange: 10.0,
            tickFrequency: 10,
            isSnapToTickEnabled: false,
            tickPlacement: SliderControlTickPlacement.BottomRight,
            showTextField: true,
            isTextFieldEditable: true,
            textValidationRegex: "\\d{1-3}")]
        public int IntSlider { get; set; } = 100;

        [DisplayName("Double Slider")]
        [Description("This is a double that uses a slider control without any frills.")]
        [DefaultValue(0.5)]
        [SliderControlParams(minimum: 0.0, maximum: 1.0)]
        public double DoubleSlider { get; set; } = 0.5;

        [DisplayName("File Picker")]
        [Description("This is a sample file picker.")]
        [DefaultValue("")]
        [FilePickerParams(title: "Choose a File to load from")]
        public string File { get; set; } = "";

        [DisplayName("Folder Picker")]
        [Description("Opens a file picker but locked to only allow folder selections.")]
        [DefaultValue("")]
        [FolderPickerParams(
            initialFolderPath: Environment.SpecialFolder.Desktop,
            userCanEditPathText: false,
            title: "Custom Folder Select",
            okButtonLabel: "Choose Folder",
            fileNameLabel: "ModFolder",
            multiSelect: true,
            forceFileSystem: true)]
        public string Folder { get; set; } = "";

        */

        [Category("Common Settings")]
        [DisplayName("Unfrozen Title Screen")]
        [Description("Makes the title screen animation continuous instead of freezing after\n pressing START Button/ENTER Key.")]
        [DefaultValue(true)]
        public bool TitleScr { get; set; } = true;

        [Category("Common Settings")]
        [DisplayName("Demo Gameplay")]
        [Description("Enables Demo Mode that is leftover in the Windows port of the game.\nGameCube and PlayStation ports contain Demo Gameplay by default.")]
        [DefaultValue(true)]
        public bool DemoMode { get; set; } = true;

        [Category("Character Settings")]
        [DisplayName("Shadow's Chaos Emerald")]
        [Description("Chaos Emerald from Chaos Inferno has different blendings\n compared to other ports. Not to mention its model and\n blendings are completely different from SA2.\n\n'Console Style' brings the same blendings as GC/XB/PS.\n'SA2 Style' brings a similar approach to SA2's emeralds.\n\nNote: Now the emerald's model by default is changed to\n use SA2's model as well")]
        [DefaultValue(TDarkChaosEme.Untouched)]
        public TDarkChaosEme TDarkChaosEmeEnum { get; set; } = TDarkChaosEme.Untouched;

        public enum TDarkChaosEme
        {
            [Display(Name = "Original/Untouched")]
            Untouched,
            [Display(Name = "Console Style")]
            Console,
            [Display(Name = "Sonic Adventure 2 Style")]
            SA2
        }

        [Category("Common Settings")]
        [DisplayName("No Exit Prompt")]
        [Description("Kell is a God.")]
        [DefaultValue(true)]
        public bool Exit { get; set; } = true;

        [Category("Stage Settings")]
        [DisplayName("Texture Pattern Animation")]
        [Description("Makes use of texture patern animation (similar to PS2 port) rather\n than indirect shaders.")]
        [DefaultValue(false)]
        public bool IndirectOFF { get; set; } = false;

    }

    /// <summary>
    /// Allows you to override certain aspects of the configuration creation process (e.g. create multiple configurations).
    /// Override elements in <see cref="ConfiguratorMixinBase"/> for finer control.
    /// </summary>
    public class ConfiguratorMixin : ConfiguratorMixinBase
    {
        // 
    }
}
