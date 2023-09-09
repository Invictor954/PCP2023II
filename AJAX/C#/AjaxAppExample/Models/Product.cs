using System;
using System.Collections.Generic;

namespace AjaxAppExample.Models;

public partial class Product
{
    public int ProductId { get; set; }

    public string Name { get; set; } = null!;

    public decimal Price { get; set; }

    public int BrandId { get; set; }

    public virtual Brand Brand { get; set; } = null!;
}
