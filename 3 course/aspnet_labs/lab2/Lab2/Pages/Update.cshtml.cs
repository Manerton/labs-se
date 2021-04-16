using System;
using Lab2.Data;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.AspNetCore.Mvc;
using System.Collections.Generic;

namespace Lab2.Pages
{
    public class UpdateModel : PageModel
    {
        private ToDoListRepositary repositary;
        public ToDoListItem updatingItem;
        public UpdateModel(ToDoListRepositary repositary)
        {
            this.repositary = repositary;
        }
        public void OnGet(Guid Id)
        {
            updatingItem = repositary.GetItemById(Id);
        }
        public IActionResult OnPost(Guid Id, string name, bool done, Int16 priority)
        {
            repositary.Update(new ToDoListItem(Id, name, done, priority));
            return RedirectToPage("Index");
        }
    }
}
