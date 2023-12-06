using System;
using System.Collections.Generic;
using CommonTool.Action;

namespace CommonTool
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                Parameter param = new Parameter(args);
                Console.WriteLine($"[AZ] startup common_tool");
                Console.WriteLine($"[AZ] version : v1.0.0");
                Console.WriteLine($"[AZ] args : {string.Join(" ", args)}");

                Dictionary<string, ActionBase> dicAction = new Dictionary<string, ActionBase>
                {
                    {"gen:table", new GenerateTable(param)}
                };

                if (args.Length <= 2)
                {
                    throw new Exception($"not found action. actionName: {string.Join(" ", args)}");
                }

                string actionName = args[0] + ":" + args[1];

                ActionBase action;
                if (dicAction.TryGetValue(actionName, out action) == false)
                {
                    throw new Exception($"not found action. actionName: {actionName}");
                }

                action.Run();
            }
            catch (Exception ex)
            {
                throw;
            }
        }
    }
}
