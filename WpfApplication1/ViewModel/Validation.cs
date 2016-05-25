using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using Editor.Model;
using System.ComponentModel.DataAnnotations;

namespace Editor.ViewModel
{
    public static class Validation
    {
        public class UniqueTextureName : ValidationAttribute
        {
            public UniqueTextureName()
                : base("Name must be unique") { }

            protected override System.ComponentModel.DataAnnotations.ValidationResult IsValid(object value, ValidationContext validationContext)
            {
                string str = value as string;

                if (string.IsNullOrEmpty(str))
                    return null;
                if (!DataProvider.ExistsTexture(str))
                    return null;
                return new System.ComponentModel.DataAnnotations.ValidationResult(base.FormatErrorMessage(validationContext.MemberName)
                    , new string[] { validationContext.MemberName });
            }
        }
    }
}
