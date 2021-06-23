using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Net.Http.Json;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using Lab4.Shared.Domain;
using Lab4.Shared.DTO;

namespace Lab4.Client.Services
{
    public class ToDoListItemService
    {
        private readonly HttpClient _http;

        public ToDoListItemService(HttpClient http)
        {
            _http = http;
        }

        public async Task<IEnumerable<ToDoListItemModel>> GetItems(string ListId)
        {
            return await _http.GetFromJsonAsync<IEnumerable<ToDoListItemModel>>($"api/to-do-list-item/from/{ListId}");
        }

        public async Task<ToDoListItemModel> GetItemById(string id)
        {
            return await _http.GetFromJsonAsync<ToDoListItemModel>($"api/to-do-list-item/{id}");
        }

        public async Task CreateItem(CreateToDoListItemDTO item)
        {
            var result = await _http.PostAsJsonAsync($"api/to-do-list-item", item);

            if (!result.IsSuccessStatusCode)
                throw new ApplicationException("Не получилось добавить дело в список.");
        }

        public async Task DeleteItem(ToDoListItemModel item)
        {
            var result = await _http.DeleteAsync($"api/to-do-list-item?id={item.ItemId}");

            if (!result.IsSuccessStatusCode)
                throw new ApplicationException("Не получилось удалить дело из списка.");
        }

        public async Task UpdateItem(ToDoListItemModel item)
        {
            var result = await _http.PatchAsync($"api/to-do-list-item",
                new StringContent(JsonSerializer.Serialize(item),
                Encoding.UTF8,
                "application/json"));


            if (!result.IsSuccessStatusCode)
                throw new ApplicationException("Не получилось обновить дело в списке.");
        }
    }
}
