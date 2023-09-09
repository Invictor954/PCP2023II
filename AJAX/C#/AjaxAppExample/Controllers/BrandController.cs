using AjaxAppExample.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace AjaxAppExample.Controllers
{
    public class BrandController : Controller
    {
        private readonly DbstoreContext _dbstoreContext;

        public BrandController(DbstoreContext dbstoreContext)
        {
            _dbstoreContext = dbstoreContext;
        }

        public async Task<IActionResult> Index()
        {
            return View(await _dbstoreContext.Brands.ToListAsync());
        }
    }
}
