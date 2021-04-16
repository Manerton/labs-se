using System;
using Lab2.Data;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.AspNetCore.Mvc;

namespace Lab2.Pages
{
    public class CreateModel : PageModel
    {
        private ToDoListRepositary repositary;
        public CreateModel(ToDoListRepositary repositary)
        {
            this.repositary = repositary;
        }
        public void OnGet()
        {

        }
        public IActionResult OnPost(string name, bool done, Int16 priority)
        {
            repositary.Add(new ToDoListItem(name, done, priority));
            return RedirectToPage("Index");
        }
    }
}
