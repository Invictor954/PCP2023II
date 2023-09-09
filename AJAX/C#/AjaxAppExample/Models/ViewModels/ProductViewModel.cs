using System.ComponentModel.DataAnnotations;
using System.Data.SqlTypes;

namespace AjaxAppExample.Models.ViewModels
{
    public class ProductViewModel
    {
        [Required]
        public string Name { get; set; }
        
        [Required]
        public SqlMoney Price { get; set; }

        [Required]
        public int BrandId { get; set; }
    }
}
