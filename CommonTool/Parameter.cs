using System;
using System.Collections.Generic;
using System.Text;
using CommonTool.Action;

namespace CommonTool
{
    public class Parameter
    {
        public readonly Dictionary<string, string> DicActionParam = new Dictionary<string, string>();
        public List<TableCombine> ListTableCombine = new List<TableCombine>();

        public Parameter(string[] args)
        {
            for (int i = 0; i < args.Length; ++i)
            {
                string key = args[i];
                if (i >= args.Length - 1)
                {
                    throw new System.Exception($"invalid args length - {string.Join(", ", args)}");
                }

                string value = args[++i];
                if (value.StartsWith("--") == true)
                {
                    throw new System.Exception($"invalid args value - {value}");
                }

                DicActionParam.Add(key, value);
            }
        }
    }
}
