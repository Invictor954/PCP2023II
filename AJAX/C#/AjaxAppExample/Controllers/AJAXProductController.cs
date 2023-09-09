using AjaxAppExample.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace AjaxAppExample.Controllers
{
    public class AJAXProductController : Controller
    {
        private readonly DbstoreContext _dbstoreContext;

        public AJAXProductController(DbstoreContext dbstoreContext)
        {
            _dbstoreContext = dbstoreContext;
        }
        public async Task<IActionResult> Index()
        {
            var products = _dbstoreContext.Products.Include(b => b.Brand);
            return View(await products.ToListAsync());
        }
        public ActionResult Reload()
        {
            var products = _dbstoreContext.Products.Include(b => b.Brand);
            return PartialView("_AJAXProducts", products); // Cambia "_ProductList" al nombre de tu vista parcial.
        }

    }
}
