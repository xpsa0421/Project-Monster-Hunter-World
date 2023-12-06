using System.Collections.Generic;

namespace CommonTool.Action
{
    public class TableCombine
    {
        public string TemplateName;
        public string CombineFileName;
        public List<string> ListFileName;
    }

    public class ActionBase
    {
        protected Parameter _param;

        public ActionBase(Parameter param)
        {
            _param = param;
        }

        public virtual void Run()
        {
        }
    }
}
