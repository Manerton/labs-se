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
    public class ToDoListItemController : Controller
    {
        private readonly ILogger<ToDoListItemController> _logger;
        private ToDoListItemRepository _repository;
        private ToDoListRepository _listRepository;
        public ToDoListItemController(ILogger<ToDoListItemController> logger, ToDoListItemRepository repository, ToDoListRepository listRepository)
        {
            _logger = logger;
            _repository = repository;
            _listRepository = listRepository;
        }
        public async Task<IActionResult> Index(Int64 Id)
        {
            ViewBag.ListId = Id;
            ViewBag.ListName = (await _listRepository.GetItemById(Id)).Name;
            return View(await _repository.List(Id));
        }
        [HttpGet]
        public async Task<IActionResult> Create(Int64 Id)
        {
            ViewBag.ListId = Id;
            ViewBag.ListName = (await _listRepository.GetItemById(Id)).Name;
            return View();
        }
        [HttpPost]
        public async Task<IActionResult> Create(ToDoListItemModel toDoListItem)
        {
            await _repository.Add(toDoListItem);
            return RedirectToAction("Index", new { Id = toDoListItem.ListId });
        }
        [HttpGet]
        public async Task<IActionResult> Update(Int64 Id)
        {
            var updatingItem = await _repository.GetItemById(Id);
            ViewBag.ListName = (await _listRepository.GetItemById(updatingItem.ListId)).Name;
            return View(await _repository.GetItemById(Id));
        }
        [HttpPost]
        public async Task<IActionResult> Update(ToDoListItemModel toDoListItem)
        {
            await _repository.Update(toDoListItem);
            return RedirectToAction("Index", new { Id = toDoListItem.ListId });
        }
        [HttpPost]
        public async Task<IActionResult> Delete(Int64 Id)
        {
            await _repository.Remove(Id);
            return Redirect(Request.Headers["Referer"].ToString());
        }
        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
