using Lab4.Shared.DTO;
using Lab4.Shared.Domain;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Lab4.Server.Repositories;
using System.Security.Claims;

namespace Lab4.Server.Controllers
{
    [Authorize]
    [ApiController]
    [Route("api/to-do-list")]
    public class ToDoListController : ControllerBase
    {
        private ToDoListRepository _repository;
        private readonly ILogger<ToDoListController> _logger;
        public ToDoListController(ILogger<ToDoListController> logger, ToDoListRepository repository)
        {
            _repository = repository;
            _logger = logger;
        }

        [HttpGet("")]
        public async Task<ActionResult<IEnumerable<ToDoListModel>>> GetAll()
        {
            return Ok(await _repository.List(User.FindFirstValue(ClaimTypes.NameIdentifier)));
        }

        [HttpGet("{id}")]
        public async Task<ActionResult<ToDoListModel>> GetById(Guid id)
        {
            ToDoListModel list = await _repository.GetItemById(id);
            
            if (list == null)
                return NotFound(id);

            if (list.UserId == User.FindFirstValue(ClaimTypes.NameIdentifier))
                return Ok(list);

            return Forbid();
        }

        [HttpPost]
        public async Task<ActionResult> Post(CreateToDoListDTO param)
        {
            var toDoList = new ToDoListModel
            {
                ListId = Guid.NewGuid(),
                Name = param.Name,
                UserId = User.FindFirstValue(ClaimTypes.NameIdentifier)
            };

            await _repository.Add(toDoList);

            return Created("ToDoList", toDoList);
        }

        [HttpPatch]
        public async Task<IActionResult> Patch(ToDoListModel newList)
        {
            if (newList == null)
                return NotFound();

            ToDoListModel oldList = await _repository.GetItemById(newList.ListId);

            if (oldList == null)
                return NotFound(newList.ListId);

            string userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (oldList.UserId != userId || newList.UserId != userId )
                return Forbid();

            await _repository.Update(newList);
            return Created("ToDoList", newList);
        }
        
        [HttpDelete]
        public async Task<IActionResult> Delete(Guid? id)
        {
            if (!id.HasValue)
                return NotFound();

            ToDoListModel toDoList = await _repository.GetItemById(id.Value);
            if (toDoList == null)
                return NotFound(id.Value);

            string userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (toDoList.UserId != userId)
                return Forbid();   

            await _repository.Remove(id.Value);

            return NoContent();
        }
    }
}
