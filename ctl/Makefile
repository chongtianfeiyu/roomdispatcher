include ../../lightframe/Makefile_public

ROOMDISPATCHER_DIR = $(BASE_DIR)/roomdispatcher

ROOMDISPATCHER_INCLUDE_DIR = $(ROOMDISPATCHER_DIR)

ROOMDISPATCHER_OBJ_DIR = $(ROOMDISPATCHER_DIR)/.objs

SRC = $(wildcard *.cpp)
OBJS = $(addprefix $(ROOMDISPATCHER_OBJ_DIR)/, $(subst .cpp,.o,$(SRC)))

INC = -I$(LIGHTFRAME_INCLUDE_DIR) -I$(ROOMDISPATCHER_INCLUDE_DIR) -I$(MYSQL_INCLUDE_DIR) -I$(PUBLIC_INCLUDE_DIR)

all : $(OBJS)

$(ROOMDISPATCHER_OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS)
