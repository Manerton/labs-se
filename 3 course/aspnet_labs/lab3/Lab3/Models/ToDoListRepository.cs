using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;

namespace Lab3.Models
{
    public class ToDoListRepository
    {
        private ToDoListContext _context;
        public ToDoListRepository(ToDoListContext context)
        {
            _context = context;
        }
        public async Task<IList<ToDoListModel>> List()
        {
            return await _context.Lists
                .OrderBy(x => x.ListId)
                .ToListAsync();
        }
        public async Task Add(ToDoListModel data)
        {
            await _context.Lists.AddAsync(data);
            await _context.SaveChangesAsync();
        }
        public async Task Update(ToDoListModel data)
        {
            // Find data with same id
            ToDoListModel updatingData = _context.Lists
                .Single(x => x.ListId == data.ListId);
            // Update it's attributes
            updatingData.Name = data.Name;
            await _context.SaveChangesAsync();
        }
        public async Task Remove(Int64 Id)
        {
            // Find data with same id
            ToDoListModel removingData = _context.Lists
                .Single(x => x.ListId == Id);
            // Remove data row
            _context.Lists.Remove(removingData);
            await _context.SaveChangesAsync();
        }
        public async Task<ToDoListModel> GetItemById(Int64 Id)
        {
            // Find data with same id
            return await _context.Lists
                .SingleAsync(x => x.ListId == Id);
        }
    }
}
