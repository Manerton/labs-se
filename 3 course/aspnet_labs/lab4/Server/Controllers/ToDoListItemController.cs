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
    [Route("api/to-do-list-item")]
    public class ToDoListItemController : ControllerBase
    {
        private ToDoListRepository _listRepository;
        private ToDoListItemRepository _itemRepository;
        private readonly ILogger<ToDoListItemController> _logger;
        public ToDoListItemController(ILogger<ToDoListItemController> logger, ToDoListItemRepository itemRepository, ToDoListRepository listRepository)
        {
            _listRepository = listRepository;
            _itemRepository = itemRepository;
            _logger = logger;
        }

        [HttpGet("from/{id}")]
        public async Task<ActionResult<IEnumerable<ToDoListItemModel>>> GetAll(Guid id)
        {
            ToDoListModel list = await _listRepository.GetItemById(id);

            if (list == null)
                return NotFound(id);

            if (list.UserId == User.FindFirstValue(ClaimTypes.NameIdentifier))
                return Ok(await _itemRepository.List(id));

            return Forbid();
        }

        [HttpGet("{id}")]
        public async Task<ActionResult<ToDoListItemModel>> GetById(Guid id)
        {
            ToDoListItemModel item = await _itemRepository.GetItemById(id);

            if (item == null)
                return NotFound(id);

            ToDoListModel list = await _listRepository.GetItemById(item.ListId);

            if (list.UserId == User.FindFirstValue(ClaimTypes.NameIdentifier))
                return Ok(item);

            return Forbid();
        }

        [HttpPost]
        public async Task<ActionResult> Post(CreateToDoListItemDTO param)
        {
            ToDoListModel list = await _listRepository.GetItemById(param.ListId);

            if (list == null)
                return NotFound();

            if (list.UserId != User.FindFirstValue(ClaimTypes.NameIdentifier))
                return Forbid();

            var item = new ToDoListItemModel
            {
                ItemId = Guid.NewGuid(),
                Name = param.Name,
                Done = param.Done,
                ListId = param.ListId
            };

            await _itemRepository.Add(item);

            return Created("ToDoListItem", item);
        }

        [HttpPatch]
        public async Task<IActionResult> Patch(ToDoListItemModel newItem)
        {
            if (newItem == null)
                return NotFound();

            ToDoListItemModel oldItem = await _itemRepository.GetItemById(newItem.ItemId);

            if (oldItem == null)
                return NotFound(newItem.ListId);

            ToDoListModel oldItemList = await _listRepository.GetItemById(oldItem.ListId);
            ToDoListModel newItemList = await _listRepository.GetItemById(newItem.ListId);

            string userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (oldItemList.UserId != userId || newItemList.UserId != userId)
                return Forbid();

            await _itemRepository.Update(newItem);

            return Created("ToDoListItem", newItem);
        }

        [HttpDelete]
        public async Task<IActionResult> Delete(Guid? id)
        {
            if (!id.HasValue)
                return NotFound();

            ToDoListItemModel item = await _itemRepository.GetItemById(id.Value);

            if (item == null)
                return NotFound(id.Value);

            ToDoListModel list = await _listRepository.GetItemById(item.ListId);

            string userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (list.UserId != userId)
                return Forbid();

            await _itemRepository.Remove(id.Value);

            return NoContent();
        }
    }
}
