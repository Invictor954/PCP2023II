using AjaxAppExample.Models;
using AjaxAppExample.Models.ViewModels;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;

namespace AjaxAppExample.Controllers
{
    public class ProductController : Controller
    {
        private readonly DbstoreContext _dbstoreContext;

        public ProductController(DbstoreContext dbstoreContext)
        {
            _dbstoreContext = dbstoreContext;
        }

        public async Task<IActionResult> Index()
        {
            var products = _dbstoreContext.Products.Include(b => b.Brand);
            return View(await products.ToListAsync());
        }

        public IActionResult Create()
        {
            ViewData["Brands"] = new SelectList(_dbstoreContext.Brands, "BrandId", "Name");
            return View();
        }

        [HttpPost]
        public async Task<IActionResult> Create(ProductViewModel model)
        {
            if(ModelState.IsValid)
            {
                var product = new Product()
                {
                    Name = model.Name,
                    Price = (decimal)model.Price,
                    BrandId = model.BrandId,
                };
                _dbstoreContext.Add(product);
                await _dbstoreContext.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            ViewData["Brands"] = new SelectList(_dbstoreContext.Brands, "BrandId", "Name");
            return View();
        }
    }
}
