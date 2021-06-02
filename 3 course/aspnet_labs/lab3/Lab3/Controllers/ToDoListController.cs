using Lab3.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;

namespace Lab3.Controllers
{
    public class ToDoListController : Controller
    {
        private readonly ILogger<ToDoListController> _logger;
        private ToDoListRepository _repository;
        public ToDoListController(ILogger<ToDoListController> logger, ToDoListRepository repository)
        {
            _logger = logger;
            _repository = repository;
        }

        public async Task<IActionResult> Index()
        {
            return View(await _repository.List());
        }

        [HttpGet]
        public IActionResult Create()
        {
            return View();
        }
        [HttpPost]
        public async Task<IActionResult> Create(ToDoListModel toDoList)
        {
            await _repository.Add(toDoList);
            return RedirectToAction("Index");
        }
        [HttpGet]
        public async Task<IActionResult> Update(Int64 Id)
        {
            return View(await _repository.GetItemById(Id));
        }
        [HttpPost]
        public async Task<IActionResult> Update(ToDoListModel toDoList)
        {
            await _repository.Update(toDoList);
            return RedirectToAction("Index");
        }
        [HttpPost]
        public async Task<IActionResult> Delete(Int64 Id)
        {
            await _repository.Remove(Id);
            return RedirectToAction("Index");
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
