/** Identifiers for rsource type. 
\addtogroup ResourceTypes Resource Types
\{ */
#define RES_TYPE_IMAGE		1 ///< Image resource identifier.
#define RES_TYPE_FONT		2 ///< Font resource identifier.
#define RES_TYPE_SOUND		3 ///< Sound resource identifier.
#define RES_TYPE_SPRITE		4 ///< Sprite resource identifier.
#define RES_TYPE_OBJECT		5 ///< Object resource identifier.
#define RES_TYPE_IMAGESET	6 ///< Imageset resource identifier.
/** \} */

/** Font styles.
\addtogroup FontStyles Font Styles
\{ */
#define FONT_ITALIC	1 ///< Italic font style.
#define FONT_BOLD	2 ///< Bold font style.
/** \} */

/** Global Functions.
\addtogroup GlobalFunctions Global Functions
\{ */
#define CAST_RES(type, res) static_cast<type>(res->GetResourcePtr()) ///< Casts resource type.
/** \} */

class ResourceManager;

/** 
Abstract base class for all resources.
*/
class IResource
{
public:
	/**
	 Enum Identifying the resource status. 
	 */
	enum Status
	{
		Waiting, ///< Resource is waiting to be loaded.
		Loaded ///< Resource is successfully loaded.
	};

private:
	C_Mutex mutex; ///< Mutex for thread safety of the resource status object.
	Status m_status; ///< Resource status object.
	unsigned int m_group; ///< Resource group.
	wstring m_filename; ///< File name (Path) of resource to be loaded.
	unsigned int m_type; ///< Resource type, can be one of \link ResourceTypes Resource Types \endlink.
	string m_name; ///< Name of the resource.

protected:
	/**	\name Properties
	\{	*/
	/** method to set a group of the resource.	
	 \param group Resource group. */
	void SetResourceGroup(const unsigned int group);

	/** method to set a resource status.
	\param val Resource status. */
	void SetResourceStatus(const Status val);
	
	/** method to set filename/arguments for the resource. 
	\param filename Filename (Path)/Arguments of the resource to be loaded. */
	void SetFileName(const wchar_t* filename);
	
	/** method setting the resource type.  
	\param type Resource type, can be one of a \link ResourceTypes Resource Types \endlink. */
	void SetType(const unsigned int type);
	/** \} */


	/** \name Abstract virtual methods
	\{ */
	/** Loads the resource.  
	\param mgr Pointer to the valid ResourceManager object.
	\return true if the resource is successfully loaded, false otherwise.
	*/
	virtual bool LoadFromFile(ResourceManager* mgr)=0;

	virtual void OnReset()=0; ///< Called when resetting the resource is required.
	virtual void OnLost()=0; ///< Called when the device is lost.
	/** \} */

public:
	/** Gets resource group.  
	\return Resource group of the current resource. 
	*/
	const unsigned int GetResourceGroup() const;

	/** Gets current resource status.  
	\return Current status of the resource.
	*/
	Status GetResourceStatus();

	/** Gets filename/arguments.  
	\return filename/arguments of the resource.
	*/
	const wchar_t* GetFileName() const;

	/** Gets type of the resource.  
	\return type of the resource.
	*/
	const unsigned int GetType() const;

	/** Gets name of the resource.  
	\return name of the resource
	*/
	const char* GetName() const;
	
	/** Constructor.*/
	IResource();

	/** Virtual destructor. */
	virtual ~IResource(){};

	//virtual methods
	/** \name Abstract virtual methods
	\{ */

	/** Gets resource pointer.  
	\return Resource pointer of the resource.
	\remark resource pointer returned by this method can be casted by using CAST_RES method.  
	*/
	virtual void* GetResourcePtr()=0;

	/** Returns clone of the resource without it's properties.  
	\return Resource pointer of the resource
	\warning Resource pointer returned by this method must be freed using delete keyword.
	*/
	virtual IResource* Clone()=0;

	/** Called when resources allocated by this resource must be freed. */
	virtual void Release()=0;
	/** \} */

	friend class ResourceManager; ///< friend class ResourceManager. 
};

/** Defined types.
\addtogroup TypeDef Defined Types
\{ */
typedef AbstractSmartPtr<IResource> Resource; ///< Smart resource pointer, Resource's IResource::Release method automatically called when all references to that pointer are destroyed.
/** \} */

class ResourceManager
{
protected:
	C_Device* m_dev; ///< Pointer to the device for which resources has to be loaded.
	C_SoundDevice* m_player; ///< Pointer to the sound device for which resources has to be loaded.
	C_Mutex mutex; ///< Mutex object for threadsafe compability when THREADSAFE flag is set.
	map<string, Resource > m_list; ///<  Mapped list for all of the resources.
	map<unsigned int, Resource > m_type_list; ///< List of all codecs for each type of resource.

	TSafeObject<int> LoadingStatus; ///< Thread safe integer variable to store loading status.

	/** Sets loading progress for the resources currently being loaded/processed/released.  
	\param status Current progress of resources being loaded/processed/released in the scale of 0 to 100 */
	void SetLoadingStatus(int status);

public:
	void* UserData; ///< Any type of user data to be used when loading the resource, example : File handle to load data from if it's supported by that resource loader.
	// Links
	void OnReset(); ///< Call this when required to reset all the resources.
	void OnLost(); ///< Call this when the device is lost.

	// resource builder
	/** \name Resource Builder
	\{ */
	/** Registers loader for the specified resource type.
	\param type Type of a loader to be registered, can be one of the \link ResourceTypes Resource types \endlink.
	\param res Pointer to the resource loader being registered.
	\note new keyword must be used on the resource pointer being registered, this resource loader will be automatically deleted using delete keyword when the resource manager is destructed or the resource loader is replaced by a new one.*/
	void RegisterLoader(unsigned int type, IResource* res);

	/** This method removes the resource loader specified by the type.  
	\param type Type of a loader to be registered, can be one of the \link ResourceTypes Resource types \endlink.
	\return true if the loader is removed successfully, otherwise false. */
	bool RemoveLoader(unsigned int type);
	
	/** Clears all the loaders. */
	void ClearLoaders();

	/** Creates new resource and adds it to the list.  
	\param name Name of the resource.
	\param group Group of the resource.
	\param type Type of the resource, can be one of the \link ResourceTypes Resource types \endlink.
	\param path Path/attributes of the resource.
	\return true if resource creation is successful, otherwise false.
	\note Resource name must be unique for all resources. */
	bool CreateResource(const char* name, unsigned int group, unsigned int type, const wchar_t* path);
	/** \} */

	// events
	Signal1<int> OnLoadingStatusChanged; ///< Called when resource loading status is changed with int parameter = current resource loading status.

	// resourcemanager
	/** Constructor. */
	ResourceManager();


	/** Gets the current loading status (max = 100).  
	\return current progress of resource processing. */
	int GetLoadingStatus();
	
	/** Gets the resource.  
	\param name Name of the resource being retrieved.
	\param type Type of the resource being retrieved.
	\param res [out] Pointer to the resource object for receiving the resource.
	\return true if successful, false otherwise. */
	bool GetResource(const char* name, unsigned int type, Resource* res);
	
	/** Adds the resource to the list of the resources.
	\param name Name of the resource being added to the list.
	\param res [out] Pointer of the resource being added. 
	\warning pointer passed to the res parameter must be created using new keyword, it will be automatically deleted when removing the resource. */
	void AddResource(const char* name, IResource* res);

	/** Removes the resource specified by its name from the resource list permanently.  
	\param name Name of the resource being removed
	\return true if the resource has been removed successfully, false otherwise. */
	bool RemoveResource(const char* name);

	/** Calls IResource::Release method of the specified resource but doesn't remove resource from the list.  
	\param name Name of the resource being released.
	\return true if successful, false otherwise. */
	bool ReleaseResource(const char* name);

	/** Sets the Device.  
	\param dev Device pointer. */
	void SetDevice(C_Device* dev);

	/** Gets the device associated with the resource manager.  
	\return Associated device pointer, if none returns 0. */
	C_Device* GetDevice();

	/** Sets the sound playback Device.  
	\param dev Device pointer. */
	void SetSoundDevice(C_SoundDevice* dev);

	/** Gets the sound playback device associated with the resource manager.  
	\return Associated device pointer, if none returns 0. */
	C_SoundDevice* GetSoundDevice();

	/** Releases all the resources.
	Releases all the resources by calling IResource::Release on all of the resources but does not remove them from the list for reloading them when needed. */
	void ReleaseAllResources();

	/** Releases all the resources specified by the group no.
	Releases all the resources  specified by the group no by calling IResource::Release on all of the resources found on that group but does not remove them from the list for reloading them when needed. */
	void ReleaseResourceGroup(unsigned int id);
	
	/** Releases all the resources specified by their type.
	Releases all the resources  specified by their type by calling IResource::Release on all of the resources found of that type but does not remove them from the list for reloading them when needed. */
	void ReleaseResourceType(unsigned int type);
	
	/** Loads the resource definitions from the specified file and prepares them for loading when resource loading methods are called.  
	\param filename Filename (path) of the file from which the resource definitions are being loaded.
	\return true if loading the resource definitions is successful, otherwise false. */
	bool PrepareResources(const wchar_t* filename);

	/** Loads all of the prepared resources. */
	void LoadResources();

	/** Loads all of the prepared resources specified by the group number.  
	\param id Group number of the resources to be loaded. */
	void LoadResourcesByGroup(unsigned int id);
	
	/** Loads all of the prepared resources specified by the type.  
	\param type Type of the resources to be loaded. */
	void LoadResourcesByType(unsigned int type);
};