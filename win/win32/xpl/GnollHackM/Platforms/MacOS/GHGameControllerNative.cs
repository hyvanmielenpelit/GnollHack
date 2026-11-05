#if GNH_AVALONIA && MACOS
using System;
using System.Runtime.InteropServices;

namespace GnollHackM
{
    /* Apple's GameController framework through the Objective-C runtime. The framework is loaded by path, since
       nothing else in the process loads it and its classes do not exist until it is. objc_msgSend is bound once per
       signature; BOOL returns are read as a byte (bool on arm64, signed char on x86_64). UI thread only. */
    internal static class GHGameControllerNative
    {
        private const string ObjCLibrary = "/usr/lib/libobjc.A.dylib";
        private const string GameControllerFramework = "/System/Library/Frameworks/GameController.framework/GameController";

        private static IntPtr _frameworkHandle = IntPtr.Zero;

        [DllImport(ObjCLibrary)]
        public static extern IntPtr objc_getClass(string name);

        [DllImport(ObjCLibrary)]
        public static extern IntPtr sel_registerName(string name);

        [DllImport(ObjCLibrary)]
        public static extern IntPtr object_getClassName(IntPtr obj);

        [DllImport(ObjCLibrary)]
        public static extern IntPtr objc_retain(IntPtr obj);

        [DllImport(ObjCLibrary)]
        public static extern void objc_release(IntPtr obj);

        [DllImport(ObjCLibrary)]
        public static extern IntPtr objc_autoreleasePoolPush();

        [DllImport(ObjCLibrary)]
        public static extern void objc_autoreleasePoolPop(IntPtr pool);

        [DllImport(ObjCLibrary)]
        public static extern IntPtr class_getInstanceMethod(IntPtr cls, IntPtr sel);

        [DllImport(ObjCLibrary)]
        public static extern IntPtr class_getClassMethod(IntPtr cls, IntPtr sel);

        [DllImport(ObjCLibrary, EntryPoint = "objc_msgSend")]
        public static extern IntPtr SendIntPtr(IntPtr receiver, IntPtr selector);

        [DllImport(ObjCLibrary, EntryPoint = "objc_msgSend")]
        public static extern IntPtr SendIntPtrNUInt(IntPtr receiver, IntPtr selector, nuint arg);

        [DllImport(ObjCLibrary, EntryPoint = "objc_msgSend")]
        public static extern nuint SendNUInt(IntPtr receiver, IntPtr selector);

        [DllImport(ObjCLibrary, EntryPoint = "objc_msgSend")]
        public static extern float SendFloat(IntPtr receiver, IntPtr selector);

        [DllImport(ObjCLibrary, EntryPoint = "objc_msgSend")]
        public static extern byte SendByte(IntPtr receiver, IntPtr selector);

        [DllImport(ObjCLibrary, EntryPoint = "objc_msgSend")]
        public static extern byte SendByteIntPtr(IntPtr receiver, IntPtr selector, IntPtr arg);

        /* Selectors, registered on first use after TryLoadFramework */
        public static class Sel
        {
            public static readonly IntPtr Controllers = sel_registerName("controllers");
            public static readonly IntPtr Count = sel_registerName("count");
            public static readonly IntPtr ObjectAtIndex = sel_registerName("objectAtIndex:");
            public static readonly IntPtr ExtendedGamepad = sel_registerName("extendedGamepad");
            public static readonly IntPtr VendorName = sel_registerName("vendorName");
            public static readonly IntPtr ProductCategory = sel_registerName("productCategory");
            public static readonly IntPtr UTF8String = sel_registerName("UTF8String");
            public static readonly IntPtr IsKindOfClass = sel_registerName("isKindOfClass:");
            public static readonly IntPtr ButtonA = sel_registerName("buttonA");
            public static readonly IntPtr ButtonB = sel_registerName("buttonB");
            public static readonly IntPtr ButtonX = sel_registerName("buttonX");
            public static readonly IntPtr ButtonY = sel_registerName("buttonY");
            public static readonly IntPtr LeftShoulder = sel_registerName("leftShoulder");
            public static readonly IntPtr RightShoulder = sel_registerName("rightShoulder");
            public static readonly IntPtr LeftTrigger = sel_registerName("leftTrigger");
            public static readonly IntPtr RightTrigger = sel_registerName("rightTrigger");
            public static readonly IntPtr ButtonOptions = sel_registerName("buttonOptions");
            public static readonly IntPtr ButtonMenu = sel_registerName("buttonMenu");
            public static readonly IntPtr LeftThumbstickButton = sel_registerName("leftThumbstickButton");
            public static readonly IntPtr RightThumbstickButton = sel_registerName("rightThumbstickButton");
            public static readonly IntPtr Dpad = sel_registerName("dpad");
            public static readonly IntPtr LeftThumbstick = sel_registerName("leftThumbstick");
            public static readonly IntPtr RightThumbstick = sel_registerName("rightThumbstick");
            public static readonly IntPtr XAxis = sel_registerName("xAxis");
            public static readonly IntPtr YAxis = sel_registerName("yAxis");
            public static readonly IntPtr Up = sel_registerName("up");
            public static readonly IntPtr Down = sel_registerName("down");
            public static readonly IntPtr Left = sel_registerName("left");
            public static readonly IntPtr Right = sel_registerName("right");
            public static readonly IntPtr Value = sel_registerName("value");
            public static readonly IntPtr IsPressed = sel_registerName("isPressed");
            public static readonly IntPtr IsAnalog = sel_registerName("isAnalog");
        }

        public static bool TryLoadFramework(out string error)
        {
            error = null;
            if (_frameworkHandle == IntPtr.Zero)
            {
                IntPtr handle;
                if (!NativeLibrary.TryLoad(GameControllerFramework, out handle))
                {
                    error = "GameController.framework not loaded";
                    return false;
                }
                _frameworkHandle = handle;
            }
            if (objc_getClass("GCController") == IntPtr.Zero)
            {
                error = "GCController class not found";
                return false;
            }
            return true;
        }

        /* A message to an object lacking the selector raises an Objective-C exception that managed code cannot
           catch, so every selector the backend sends is checked before it starts. Returns "Class.selector" for the
           first missing one, or null. */
        public static string FindMissingSelector()
        {
            string missing;
            if ((missing = CheckClassMethods("GCController", "controllers")) != null)
                return missing;
            if ((missing = CheckInstanceMethods("GCController", "extendedGamepad", "vendorName", "productCategory")) != null)
                return missing;
            if ((missing = CheckInstanceMethods("GCExtendedGamepad", "buttonA", "buttonB", "buttonX", "buttonY", "leftShoulder",
                "rightShoulder", "leftTrigger", "rightTrigger", "buttonOptions", "buttonMenu", "leftThumbstickButton",
                "rightThumbstickButton", "dpad", "leftThumbstick", "rightThumbstick")) != null)
                return missing;
            if ((missing = CheckInstanceMethods("GCControllerButtonInput", "value", "isPressed")) != null)
                return missing;
            if ((missing = CheckInstanceMethods("GCControllerElement", "isAnalog")) != null)
                return missing;
            if ((missing = CheckInstanceMethods("GCControllerAxisInput", "value")) != null)
                return missing;
            return CheckInstanceMethods("GCControllerDirectionPad", "xAxis", "yAxis", "up", "down", "left", "right");
        }

        private static string CheckClassMethods(string className, params string[] selectors)
        {
            IntPtr cls = objc_getClass(className);
            int i;
            for (i = 0; i < selectors.Length; i++)
            {
                if (cls == IntPtr.Zero || class_getClassMethod(cls, sel_registerName(selectors[i])) == IntPtr.Zero)
                    return className + "." + selectors[i];
            }
            return null;
        }

        private static string CheckInstanceMethods(string className, params string[] selectors)
        {
            IntPtr cls = objc_getClass(className);
            int i;
            for (i = 0; i < selectors.Length; i++)
            {
                if (cls == IntPtr.Zero || class_getInstanceMethod(cls, sel_registerName(selectors[i])) == IntPtr.Zero)
                    return className + "." + selectors[i];
            }
            return null;
        }

        public static string NSStringToString(IntPtr nsString)
        {
            if (nsString == IntPtr.Zero)
                return null;
            IntPtr utf8 = SendIntPtr(nsString, Sel.UTF8String);
            return utf8 == IntPtr.Zero ? null : Marshal.PtrToStringUTF8(utf8);
        }

        public static string GetClassName(IntPtr obj)
        {
            if (obj == IntPtr.Zero)
                return null;
            IntPtr name = object_getClassName(obj);
            return name == IntPtr.Zero ? null : Marshal.PtrToStringUTF8(name);
        }

        public static bool IsKindOf(IntPtr obj, IntPtr cls)
        {
            return obj != IntPtr.Zero && cls != IntPtr.Zero && SendByteIntPtr(obj, Sel.IsKindOfClass, cls) != 0;
        }
    }
}
#endif
