#if GNH_AVALONIA && LINUX
using System;
using Xilium.CefGlue;

namespace GnollHackM
{
    /* Runs in CEF's renderer process: defines window.GnollHackBridge in every main frame before page scripts run, and
       evaluates scripts for the browser process. Never touches GHApp, Avalonia or MAUI; logs only to standard error. */
    internal sealed class GHCefRenderProcessHandler : CefRenderProcessHandler
    {
        /* Renderer to browser: (string text, string frame URL) */
        public const string BridgeMessageName = "gnollhack_bridge";
        /* Browser to renderer: (int id, string script) */
        public const string EvalMessageName = "gnollhack_eval";
        /* Renderer to browser: (int id, bool success, string JSON or error) */
        public const string EvalResultMessageName = "gnollhack_eval_result";

        /* Renderer main thread only */
        private static GHCefBridgeV8Handler _bridgeHandler;

        /* Right after a frame's V8 context is created. The bridge is defined outside the page's scripts, so the page's
           Content Security Policy does not apply to it; the browser side decides whether its messages are used. */
        protected override void OnContextCreated(CefBrowser browser, CefFrame frame, CefV8Context context)
        {
            try
            {
                using (CefObjectTracker.StartTracking())
                {
                    if (frame == null || context == null || !frame.IsMain)
                        return;

                    if (!context.Enter())
                        return;

                    try
                    {
                        if (_bridgeHandler == null)
                            _bridgeHandler = new GHCefBridgeV8Handler();

                        CefV8Value bridge = CefV8Value.CreateObject();
                        CefV8Value onWebMessage = CefV8Value.CreateFunction("onWebMessage", _bridgeHandler);
                        bridge.SetValue("onWebMessage", onWebMessage, CefV8PropertyAttribute.ReadOnly | CefV8PropertyAttribute.DontDelete);
                        context.GetGlobal().SetValue("GnollHackBridge", bridge, CefV8PropertyAttribute.ReadOnly | CefV8PropertyAttribute.DontEnum | CefV8PropertyAttribute.DontDelete);
                    }
                    finally
                    {
                        context.Exit();
                    }
                }
            }
            catch (Exception ex)
            {
                GHCefSubprocess.WriteError("GHCefRenderProcessHandler: cannot define GnollHackBridge: " + ex.Message);
            }
        }

        protected override bool OnProcessMessageReceived(CefBrowser browser, CefFrame frame, CefProcessId sourceProcess, CefProcessMessage message)
        {
            try
            {
                using (CefObjectTracker.StartTracking())
                {
                    if (message == null || frame == null || message.Name != EvalMessageName)
                        return false;

                    Evaluate(frame, message);
                    return true;
                }
            }
            catch (Exception ex)
            {
                GHCefSubprocess.WriteError("GHCefRenderProcessHandler: cannot handle a process message: " + ex.Message);
                return false;
            }
        }

        /* Evaluates the script in the frame's context and replies with the result as JSON (a string result as it is),
           or with the error message */
        private static void Evaluate(CefFrame frame, CefProcessMessage message)
        {
            CefListValue arguments = message.Arguments;
            if (arguments == null)
                return;

            int id = arguments.GetInt(0);
            string script = arguments.GetString(1) ?? "";
            bool success = false;
            string result;

            try
            {
                CefV8Context context = frame.V8Context;
                if (context == null || !context.IsValid)
                {
                    result = "no script context";
                }
                else if (!context.Enter())
                {
                    result = "cannot enter the script context";
                }
                else
                {
                    try
                    {
                        CefV8Value value;
                        CefV8Exception exception;
                        if (context.TryEval(script, frame.Url, 1, out value, out exception))
                        {
                            result = ToJson(context, value, out success);
                        }
                        else
                        {
                            result = exception != null ? exception.Message : "evaluation failed";
                        }
                    }
                    finally
                    {
                        context.Exit();
                    }
                }
            }
            catch (Exception ex)
            {
                success = false;
                result = ex.Message;
            }

            CefProcessMessage reply = CefProcessMessage.Create(EvalResultMessageName);
            CefListValue replyArguments = reply.Arguments;
            replyArguments.SetInt(0, id);
            replyArguments.SetBool(1, success);
            replyArguments.SetString(2, result ?? "");
            frame.SendProcessMessage(CefProcessId.Browser, reply);
        }

        /* JSON.stringify of the value, from the page's own global; undefined and unserializable values give "null" */
        private static string ToJson(CefV8Context context, CefV8Value value, out bool success)
        {
            success = true;
            if (value == null || value.IsUndefined || value.IsNull)
                return "null";
            if (value.IsString)
                return value.GetStringValue();

            CefV8Value json = context.GetGlobal().GetValue("JSON");
            CefV8Value stringify = json != null && json.IsObject ? json.GetValue("stringify") : null;
            if (stringify == null || !stringify.IsFunction)
            {
                success = false;
                return "JSON.stringify is not available";
            }

            CefV8Value text = stringify.ExecuteFunction(json, new CefV8Value[] { value });
            if (text == null)
            {
                success = false;
                CefV8Exception exception = stringify.HasException ? stringify.GetException() : null;
                return exception != null ? exception.Message : "JSON.stringify failed";
            }
            return text.IsString ? text.GetStringValue() : "null";
        }
    }

    /* window.GnollHackBridge.onWebMessage(s): sends String(s) and the calling frame's URL to the browser process.
       CefV8Handler's native thunk tracks and disposes the proxies of each call. */
    internal sealed class GHCefBridgeV8Handler : CefV8Handler
    {
        protected override bool Execute(string name, CefV8Value obj, CefV8Value[] arguments, out CefV8Value returnValue, out string exception)
        {
            returnValue = null;
            exception = null;
            try
            {
                if (arguments == null || arguments.Length != 1)
                {
                    exception = "GnollHackBridge.onWebMessage takes one argument";
                    return true;
                }

                CefV8Context context = CefV8Context.GetCurrentContext();
                string text = ToText(context, arguments[0]);
                CefFrame frame = context.GetFrame();
                if (frame == null)
                    return true;

                CefProcessMessage message = CefProcessMessage.Create(GHCefRenderProcessHandler.BridgeMessageName);
                CefListValue messageArguments = message.Arguments;
                messageArguments.SetString(0, text);
                messageArguments.SetString(1, frame.Url ?? "");
                frame.SendProcessMessage(CefProcessId.Browser, message);
                return true;
            }
            catch (Exception ex)
            {
                GHCefSubprocess.WriteError("GHCefBridgeV8Handler: " + ex.Message);
                exception = "GnollHackBridge.onWebMessage failed";
                return true;
            }
        }

        /* The string itself, or JavaScript's String(value) for anything else */
        private static string ToText(CefV8Context context, CefV8Value value)
        {
            if (value == null)
                return "";
            if (value.IsString)
                return value.GetStringValue();

            CefV8Value stringFunction = context.GetGlobal().GetValue("String");
            if (stringFunction != null && stringFunction.IsFunction)
            {
                CefV8Value text = stringFunction.ExecuteFunction(null, new CefV8Value[] { value });
                if (text != null && text.IsString)
                    return text.GetStringValue();
            }
            return value.GetStringValue() ?? "";
        }
    }
}
#endif
