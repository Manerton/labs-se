using System;
using Lab2.Data;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.AspNetCore.Mvc;

namespace Lab2.Pages
{
    public class IndexModel : PageModel
    {
        public ToDoListRepositary repositary;
        public IndexModel(ToDoListRepositary repositary)
        {
            this.repositary = repositary;
        }

        public void OnGet()
        {

        }
        public IActionResult OnPostRemove(Guid id)
        {
            repositary.Remove(id);
            return RedirectToPage();
        }
    }
}
